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

#ifndef hpc_containers_global_hh
#define hpc_containers_global_hh

#include <ostream>
#include <tuple>
#include <boost/lexical_cast.hpp>

namespace hpc {

   template< class T,
	     class U >
   std::ostream&
   operator<<( std::ostream& strm,
	       const std::pair<T,U>& obj )
   {
      strm << "(" << obj.first << ", " << obj.second << ")";
      return strm;
   }

   template< class T,
	     class U,
             class V >
   std::ostream&
   operator<<( std::ostream& strm,
               const std::tuple<T,U,V>& obj )
   {
      strm << "(" << std::get<0>( obj ) << ", " << std::get<1>( obj ) << ", " << std::get<2>( obj ) << ")";
      return strm;
   }

   ///
   /// Convert anything to a string.
   ///
   template< class Source >
   std::string
   to_string( Source src )
   {
      return boost::lexical_cast<std::string>( src );
   }
}

#endif
