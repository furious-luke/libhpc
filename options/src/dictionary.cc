// Copyright 2012 Luke Hodkinson

// This file is part of libhpc.
// 
// libhpc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// libhpc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with libhpc.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/algorithm/string.hpp>
#include "dictionary.hh"

namespace hpc {
   namespace options {

      dictionary::dictionary( const hpc::string& prefix )
         : _pre( prefix ),
           _sep( ":" )
      {
	 std::transform( _pre.begin(), _pre.end(), _pre.begin(), ::tolower );
      }

      void
      dictionary::clear()
      {
         _opts.deallocate();
         _dicts.deallocate();
         _opts_mm.clear();
         _dicts_mm.clear();
      }

      const dictionary&
      dictionary::sub( const hpc::string& prefix ) const
      {
	 ASSERT( find_sub( prefix ) );
	 return *find_sub( prefix );
      }

      dictionary&
      dictionary::sub( const hpc::string& prefix )
      {
	 ASSERT( find_sub( prefix ) );
	 return *find_sub( prefix );
      }

      void
      dictionary::add_option( option_base* opt,
                              optional<const hpc::string&> prefix )
      {
         dictionary* sub;
         if( prefix )
         {
	    // Lower case the prefix.
	    hpc::string pre( *prefix );
	    std::transform( pre.begin(), pre.end(), pre.begin(), ::tolower );

	    // Split the prefix on our separator.
	    vector<hpc::string> levels;
	    boost::split( levels, pre, boost::is_any_of( _sep ) );

	    // Traverse the levels, trying to either locate or
	    // create a dictionary, starting with this dictionary.
	    dictionary* cur_dict = this;
	    for( const auto& level_name : levels )
	    {
	       // Unfortunately I need to use a linear search here,
	       // because at this stage the dictionary will not be
	       // compiled.
	       // TODO: Fix this.
	       sub = NULL;
	       for( unsigned ii = 0; ii < cur_dict->_dicts.size(); ++ii )
	       {
		  if( cur_dict->_dicts[ii]->_pre == level_name )
		  {
		     sub = cur_dict->_dicts[ii];
		     break;
		  }
	       }

	       // Create the dictionary if it does not already exist.
	       if( !sub )
	       {
		  sub = new dictionary( level_name );
		  cur_dict->add_dictionary( sub );
	       }

	       // Swap to the latest dictinoary.
	       cur_dict = sub;
	    }
         }
         else
            sub = this;

#ifndef NDEBUG
         for( const auto& exist : sub->_opts )
            ASSERT( exist->name() != opt->name() );
#endif
         sub->_opts.push_back( opt ); // TODO: Fix slowness maybe?
         sub->_opts_mm.add_match( opt->name() );
         sub->_ready = false;
      }

      void
      dictionary::add_option( option_base* opt,
                              const char* prefix )
      {
         add_option( opt, hpc::string( prefix ) );
      }

      void
      dictionary::add_dictionary( dictionary* dict )
      {
#ifndef NDEBUG
         for( const auto& exist : _dicts )
            ASSERT( exist->prefix() != dict->prefix() );
#endif
         _dicts.push_back( dict );
         _dicts_mm.add_match( dict->prefix() + _sep );
         _ready = false;
      }

      const hpc::string&
      dictionary::prefix() const
      {
         return _pre;
      }

      bool
      dictionary::has_option( const hpc::string& name ) const
      {
         ASSERT( _ready, "Dictionary has not been compiled." );

	 hpc::string nm( name );
	 std::transform( nm.begin(), nm.end(), nm.begin(), ::tolower );

         optional<index> idx = _opts_mm.match( nm );
         if( idx )
            return true;

         re::match match;
         idx = _dicts_mm.search( nm, match );
         if( idx && *idx != (unsigned short)-1 )
            return (*_dicts[*idx]).has_option( nm.c_str() + match.capture( *idx ).second );

         return false;
      }

      void
      dictionary::compile()
      {
         if( !_ready )
         {
            _opts_mm.compile();
            _dicts_mm.compile();
            _ready = true;
         }
         for( unsigned ii = 0; ii < _dicts.size(); ++ii )
            _dicts[ii]->compile();
      }

      vector<shared_ptr<option_base>>::const_iterator
      dictionary::options_cbegin() const
      {
         return _opts.cbegin();
      }

      vector<shared_ptr<option_base>>::iterator
      dictionary::options_begin()
      {
         return _opts.begin();
      }

      vector<shared_ptr<option_base>>::const_iterator
      dictionary::options_cend() const
      {
         return _opts.cend();
      }

      vector<shared_ptr<option_base>>::iterator
      dictionary::options_end()
      {
         return _opts.end();
      }

      vector<shared_ptr<dictionary>>::const_iterator
      dictionary::dicts_begin() const
      {
         return _dicts.cbegin();
      }

      vector<shared_ptr<dictionary>>::const_iterator
      dictionary::dicts_end() const
      {
         return _dicts.cend();
      }

      const option_base*
      dictionary::find( const hpc::string& name ) const
      {
         ASSERT( _ready, "Dictionary has not been compiled." );

	 hpc::string nm( name );
	 std::transform( nm.begin(), nm.end(), nm.begin(), ::tolower );

         optional<index> idx = _opts_mm.match( nm );
         if( idx )
            return _opts[*idx];

         re::match match;
         idx = _dicts_mm.search( nm, match );
         if( idx && *idx != (unsigned short)-1 )
	 {
	    auto cap = match.capture( *idx );
            return (*_dicts[*idx]).find( nm.c_str() + cap.second );
	 }

         return NULL;
      }

      const dictionary*
      dictionary::find_sub( const hpc::string& prefix ) const
      {
	 // Lower-case the prefix.
	 hpc::string pre( prefix );
	 std::transform( pre.begin(), pre.end(), pre.begin(), ::tolower );

         re::match match;
         optional<index> idx = _dicts_mm.search( pre + _sep, match );
         if( idx && *idx != (unsigned short)-1 )
	 {
	    const dictionary* sub = _dicts[*idx];
	    const dictionary* next = sub->find_sub( pre.c_str() + match.capture( *idx ).second );
            return next ? next : sub;
	 }
         else
            return NULL;
      }

      dictionary*
      dictionary::find_sub( const hpc::string& prefix )
      {
	 return (dictionary*)find_sub( prefix );
      }

      const option_base&
      dictionary::operator[]( const hpc::string& name ) const
      {
         ASSERT( has_option( name ), "No option by that name." );
         return *find( name );
      }

      option_base&
      dictionary::operator[]( const hpc::string& name )
      {
         return (option_base&)(*(const dictionary*)this)[name];
      }

      std::ostream&
      operator<<( std::ostream& strm,
		  const dictionary& obj )
      {
         if( !obj._pre.empty() )
            strm << "<" << obj._pre << ">";
         strm << "{";
	 bool any_opts = false;
	 if( obj._opts.size() )
         {
	    auto it = obj._opts.begin();
            while( it != obj._opts.end() && !(*it)->has_value() )
               ++it;
            if( it != obj._opts.end() )
            {
               strm << (*it)->name() << ": " << (*it)->store();
               ++it;
	       any_opts = true;
            }
	    for( ; it != obj._opts.end(); ++it )
            {
               if( !(*it)->has_value() )
                  continue;
	       strm << ", " << (*it)->name() << ": " << (*it)->store();
	       any_opts = true;
            }
	 }
	 if( obj._dicts.size() )
	 {
	    if( any_opts )
	       strm << ", ";
	    auto it = obj._dicts.begin();
	    strm << **it++;
	    while( it != obj._dicts.end() )
	       strm << ", " << **it++;
         }
	 strm << "}";
	 return strm;
      }
   }
}
