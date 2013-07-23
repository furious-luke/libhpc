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

#ifndef libhpc_containers_array3_hh
#define libhpc_containers_array3_hh

#include "array.hh"

namespace hpc {

   template< class T >
   class array< T, 3 >
      : public impl::boost::array< T, 3 >
   {
   public:

      typedef typename impl::boost::array<T,3>::size_type size_type;
      typedef index key_type;
      typedef T mapped_type;

   public:

      array()
      {
      }

      array( const T& x,
             const T& y,
             const T& z )
      {
         this->elems[0] = x;
         this->elems[1] = y;
         this->elems[2] = z;
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
                  const array& obj )
      {
	 strm << "[" << obj.elems[0] << ", " << obj.elems[1] << ", " << obj.elems[2] << "]";
	 return strm;
      }
   };

}

#endif
