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

#ifndef libhpc_containers_options_hh
#define libhpc_containers_options_hh

#include <string>
#include <boost/any.hpp>
#include "libhpc/debug/debug.hh"
#include "map.hh"

namespace hpc {
   namespace options {

      ///
      /// TODO: May need to rewrite boost::any, as it uses exceptions.
      ///
      class dictionary
         : map<std::string,boost::any>
      {
      public:

         typedef map<std::string,boost::any> super_type;

      public:

         // TODO: Fix the long iterator name.
         template< class T >
         std::pair<typename std::map<std::string,boost::any>::iterator,bool>
         insert( const std::string& name,
                 const T& value )
         {
            boost::any any_val = value;
            return super_type::insert( name, any_val );
         }

         template< class T >
         T
         get( const std::string& name )
         {
            return boost::any_cast<T>( super_type::get( name ) );
         }
      };
   }
}

#endif
