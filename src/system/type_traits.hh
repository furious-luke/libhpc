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

#ifndef libhpc_system_type_traits_hh
#define libhpc_system_type_traits_hh

#include <vector>
#include <boost/type_traits/integral_constant.hpp>
#ifdef HAVE_THRUST
#include <thrust/host_vector.h>
#endif

namespace hpc {

   template< class T >
   struct type_traits
   {
      typedef       T  value;
      typedef const T  const_value;
      typedef       T& reference;
      typedef const T& const_reference;
   };

   template< class T >
   struct random_access_trait
      : boost::false_type {};

   template< class T,
             class Alloc >
   struct random_access_trait< std::vector<T,Alloc> >
      : boost::true_type {};

#ifdef HAVE_THRUST

   template< class T,
             class Alloc >
   struct random_access_trait< thrust::host_vector<T,Alloc> >
      : boost::true_type {};

#endif

   // {
   //    template< class U >
   //    struct enabler {};

   //    template< class U >
   //    static char
   //    test( enabler<U,
   // };

}

#endif
