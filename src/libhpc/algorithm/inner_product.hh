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

#ifndef hpc_algorithm_inner_product_hh
#define hpc_algorithm_inner_product_hh

namespace hpc {

   template< class Iterator1,
	     class Iterator2 >
   typename std::iterator_traits<Iterator1>::value_type
   inner_product( Iterator1 first1,
		  const Iterator1& last1,
		  Iterator2 first2 )
   {
      typedef typename std::iterator_traits<Iterator1>::value_type real_type;
      real_type sum = 0.0;
      while( first1 != last1 )
	 sum += (*first1++)*(*first2++);
      return sum;
   }

}

#endif
