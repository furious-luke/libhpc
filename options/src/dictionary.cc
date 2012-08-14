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

#include "dictionary.hh"

namespace hpc {
   namespace options {

      dictionary::dictionary( const hpc::string& prefix )
         : _pre( prefix )
      {
      }

      void
      dictionary::add_option( option_base* opt )
      {
#ifndef NDEBUG
         for( const auto& exist : _opts )
            ASSERT( exist->name() != opt->name() );
#endif
         _opts.push_back( opt );
      }

      void
      dictionary::add_dictionary( dictionary* dict )
      {
         _dicts.push_back( dict );
      }

      const hpc::string&
      dictionary::prefix() const
      {
         return _pre;
      }

      const option_base&
      dictionary::operator[]( const hpc::string& name ) const
      {
         for( const auto& opt : _opts )
         {
            if( opt->name() == name )
               return *opt;
         }
         for( const auto& dict : _dicts )
         {
            // if( dict->find(
         }
         ASSERT( 0 );
      }

      option_base&
      dictionary::operator[]( const hpc::string& name )
      {
         for( auto& opt : _opts )
         {
            if( opt->name() == name )
               return *opt;
         }
         ASSERT( 0 );
      }

      std::ostream&
      operator<<( std::ostream& strm,
		  const dictionary& obj )
      {
         strm << "{";
	 if( obj._opts.size() )
         {
	    auto it = obj._opts.begin();
            while( !(*it)->has_value() )
               ++it;
	    strm << (*it)->name() << ": " << (*it)->store();
	    ++it;
	    for( ; it != obj._opts.end(); ++it )
            {
               if( !(*it)->has_value() )
                  continue;
	       strm << ", " << (*it)->name() << ": " << (*it)->store();
            }
	 }
         for( const auto dict : obj._dicts )
            strm << *dict;
	 strm << "}";
	 return strm;
      }
   }
}
