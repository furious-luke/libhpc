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
         const T&
         get( const hpc::string& name ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
            ASSERT( opt.get(), "Value has not been set and no default." );
            return (const T&)*opt.get();
         }

         template< class T >
         const T&
         get( const hpc::string& name,
              const T& default_value ) const
         {
            typedef typename boost::mpl::at<type_map,T>::type option_type;
            const option_type& opt = (const option_type&)((*this)[name]);
            if( opt.get() )
               return (const T&)*opt.get();
            else
               return default_value;
         }

         const dictionary&
         sub( const hpc::string& prefix ) const;

         dictionary&
         sub( const hpc::string& prefix );

         void
         add_option( option_base* opt,
                     optional<const hpc::string&> prefix=optional<const hpc::string&>() );

         void
         add_dictionary( dictionary* dict );

         void
         compile();

         const hpc::string&
         prefix() const;

         bool
         has_option( const hpc::string& name ) const;

         vector<shared_ptr<option_base>>::const_iterator
         options_begin() const;

         vector<shared_ptr<option_base>>::const_iterator
         options_end() const;

         vector<shared_ptr<dictionary>>::const_iterator
         dicts_begin() const;

         vector<shared_ptr<dictionary>>::const_iterator
         dicts_end() const;

         const option_base*
         find( const hpc::string& name ) const;

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
