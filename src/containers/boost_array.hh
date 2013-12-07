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

#ifndef libhpc_containers_boost_array_hh
#define libhpc_containers_boost_array_hh

#include "libhpc/memory/memory.hh"
#include <boost/array.hpp>

namespace hpc {
   namespace impl {
      namespace boost {

         template< class T,
                   int N >
         class array
            : public ::boost::array< T, N >
         {
         // public:

         //    typedef typename ::boost::array<T,N>::size_type size_type;
         //    typedef typename ::boost::array<T,N>::const_reference const_reference;
         //    typedef typename ::boost::array<T,N>::reference reference;
         };
      }
   }
}

#endif
