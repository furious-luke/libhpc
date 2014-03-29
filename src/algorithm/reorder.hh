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

#ifndef libhpc_algorithm_reorder_hh
#define libhpc_algorithm_reorder_hh

namespace hpc {

   template< class DataIter,
	     class OrderIter >
   void
   reorder( DataIter data_begin,
	    DataIter const& data_end,
	    OrderIter order_begin )
   {
      typedef typename std::iterator_traits<DataIter>::value_type value_type;
      typedef typename std::iterator_traits<OrderIter>::value_type index_type;
      typedef typename std::iterator_traits<DataIter>::difference_type diff_type;

      diff_type remaining = data_end - 1 - data_begin;
      for( index_type s = index_type(), d; remaining > 0; ++ s )
      {
	 for( d = order_begin[s]; d > s; d = order_begin[d] );
	 if( d == s )
	 {
	    --remaining;
	    value_type temp = data_begin[s];
	    while( d = order_begin[d], d != s )
	    {
	       std::swap( temp, data_begin[d] );
	       --remaining;
	    }
	    data_begin[s] = temp;
	 }
      }
   }

}

#endif
