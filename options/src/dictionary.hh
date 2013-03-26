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

#ifndef libhpc_options_dictionary_hh
#define libhpc_options_dictionary_hh

#include <boost/lexical_cast.hpp>
#include <boost/spirit/home/support/detail/hold_any.hpp>
#include "libhpc/debug/debug.hh"
#include "libhpc/containers/containers.hh"
#include "libhpc/regexp/multimatch.hh"
#include "option.hh"

namespace hpc {
   namespace options {

      class bad_option
         : public std::exception
      {
      public:

         bad_option( const hpc::string& option_name );

         virtual
         ~bad_option() throw();

         virtual const char*
         what() const throw();

      public:

         hpc::string _name;
         hpc::string _msg;
      };

      class no_value
         : public bad_option
      {
      public:

         no_value( const hpc::string& option_name );

         virtual
         ~no_value() throw();
      };

      ///
      ///
      ///
      class dictionary
      {
         friend std::ostream&
         operator<<( std::ostream& strm,
                     const dictionary& obj );

      public:

         dictionary( const hpc::string& prefix = hpc::string() );

         void
         clear();

         template< class T >
         const T
         get( const hpc::string& name ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
            if( !opt.get() )
               throw no_value( name );
            return *opt.get();
         }

         template< class T >
         const T
         get( const hpc::string& name,
              const T& default_value ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
            if( opt.get() )
               return *opt.get();
            else
               return default_value;
         }

         template< class T >
         optional<const T>
         opt( const hpc::string& name ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
	    optional<const typename option_type::value_type&> val = opt.get();
	    if( !val )
	       return none;
	    else
	    {
	       T cast_val = *val;
	       return optional<const T>( cast_val );
	    }
         }

         template< class T >
         const hpc::list<typename boost::mpl::at<type_map,T>::type::value_type>&
         get_list( const hpc::string& name ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type sub_option_type;
            typedef typename sub_option_type::value_type value_type;
            typedef options::list<sub_option_type> option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
            if( !opt.get() )
               throw no_value( name );
            const hpc::list<value_type>& ref = *opt.get();
            return ref;
         }

         const dictionary&
         sub( const hpc::string& prefix ) const;

         dictionary&
         sub( const hpc::string& prefix );

         void
         add_option( option_base* opt,
                     optional<const hpc::string&> prefix=optional<const hpc::string&>() );

         void
         add_option( option_base* opt,
                     const char* prefix );

         void
         add_dictionary( dictionary* dict );

         void
         compile();

         const hpc::string&
         prefix() const;

         bool
         has_option( const hpc::string& name ) const;

         vector<shared_ptr<option_base>>::const_iterator
         options_cbegin() const;

         vector<shared_ptr<option_base>>::iterator
         options_begin();

         vector<shared_ptr<option_base>>::const_iterator
         options_cend() const;

         vector<shared_ptr<option_base>>::iterator
         options_end();

         vector<shared_ptr<dictionary>>::const_iterator
         dicts_begin() const;

         vector<shared_ptr<dictionary>>::const_iterator
         dicts_end() const;

         const option_base*
         find( const hpc::string& name ) const;

	 const dictionary*
         find_sub( const hpc::string& prefix ) const;

         dictionary*
         find_sub( const hpc::string& prefix );

         const option_base&
         operator[]( const hpc::string& name ) const;

         option_base&
         operator[]( const hpc::string& name );

      protected:

         bool _ready;
         hpc::string _sep;
         hpc::string _pre;
         vector<shared_ptr<option_base>> _opts;
         vector<shared_ptr<dictionary>> _dicts;
         multimatch _opts_mm;
         multimatch _dicts_mm;
      };
   }
}

#endif
