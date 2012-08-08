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

#ifndef libhpc_containers_array_hh
#define libhpc_containers_array_hh

#include "boost_array.hh"
#include "global.hh"

namespace hpc {

   template< class T,
             int N >
   class array
      : public impl::boost::array< T, N >
   {
   public:

      typedef typename impl::boost::array<T,N>::size_type size_type;
      typedef index key_type;
      typedef T mapped_type;

   public:

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const array<T,N>& obj )
      {
	 strm << "[";
	 if( obj.size() )
         {
	    strm << obj[0];
	    for( index ii = 1; ii < obj.size(); ++ii )
	       strm << ", " << obj[ii];
	 }
	 strm << "]";
	 return strm;
      }
   };
};

#endif
