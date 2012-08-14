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
#include "option.hh"

namespace hpc {
   namespace options {

      ///
      ///
      ///
      class dictionary
      {
      public:

         dictionary( const hpc::string& prefix = hpc::string() );

         template< class T >
         T
         get( const hpc::string& name ) const
         {
            return (T)(((const typename boost::mpl::at<type_map,T>::type&)((*this)[name])).value());
         }

         void
         add_option( option_base* opt );

         void
         add_dictionary( dictionary* dict );

         const hpc::string&
         prefix() const;

         list<shared_ptr<option_base>>::const_iterator
         options_begin() const
         {
            return _opts.cbegin();
         }

         list<shared_ptr<option_base>>::const_iterator
         options_end() const
         {
            return _opts.cend();
         }

         list<shared_ptr<dictionary>>::const_iterator
         dicts_begin() const
         {
            return _dicts.cbegin();
         }

         list<shared_ptr<dictionary>>::const_iterator
         dicts_end() const
         {
            return _dicts.cend();
         }

         const option_base&
         operator[]( const hpc::string& name ) const;

         option_base&
         operator[]( const hpc::string& name );

         friend std::ostream&
         operator<<( std::ostream& strm,
                     const dictionary& obj );

      protected:

         hpc::string _pre;
         list<shared_ptr<option_base>> _opts;
         list<shared_ptr<dictionary>> _dicts;
      };
   }
}

#endif
