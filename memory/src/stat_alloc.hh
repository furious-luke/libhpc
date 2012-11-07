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

#ifndef libhpc_memory_stat_alloc_hh
#define libhpc_memory_stat_alloc_hh

#include <stdlib.h>
#include <memory>
#include "libhpc/debug/assert.hh"

namespace hpc {
   namespace memory {

      template< class T >
      class stat_alloc
	: public std::allocator<T>
      {
      public:

	 typedef typename std::allocator<T>::pointer pointer;
	 typedef typename std::allocator<T>::size_type size_type;

	 template< typename U >
	 struct rebind
	 {
	    typedef stat_alloc<U> other;
	 };

      public:

	 pointer
	 allocate( size_type size,
		   std::allocator<void>::const_pointer hint=0 )
	 {
	    pointer ptr = (pointer)malloc( size*sizeof(T) );
	    ASSERT( ptr );
	    return ptr;
	 }

	 void
	 deallocate( pointer ptr,
		     size_type size )
	 {
	    ASSERT( ptr );
	    free( (void*)ptr );
	 }
      };
   }
}

#endif
