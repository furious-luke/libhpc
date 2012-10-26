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

#include <stdlib.h>
// #include <stdio.h>
// #include <string.h>
#include "libhpc/debug/debug.hh"
#include "new.hh"
#include "state.hh"
#include "frag.hh"

typedef unsigned char byte_t;

void*
operator new( size_t size )
{
   return hpc::memory::new_alloc(size);
}

void
operator delete( void* ptr ) noexcept( true )
{
   hpc::memory::new_free(ptr);
}

void*
operator new[]( size_t size )
{
   return hpc::memory::new_alloc(size);
}

void
operator delete[]( void* ptr ) noexcept( true )
{
   hpc::memory::new_free(ptr);
}

namespace hpc {
   namespace memory {

      static const int ptr_cookie_size = 4;
      static const int ptr_cookie_byte = 0xef;

      extern debug::group_context<memory::state_t> ctx;

#ifndef NMEMSTATS

      void
      update_stats( size_t size )
      {
	 for(size_t ii = 0; ii < memory::ctx.num_selected(); ++ii) {
	    memory::state_t& state = memory::ctx.selected_group(ii).data();
	    state.size += size;
	    if(state.size > state.peak)
	       state.peak = state.size;
	 }
      }

#endif

      void*
      new_alloc( size_t size )
      {
#ifndef NMEMSTATS
	 update_stats(size);
#endif

#ifdef NDEBUG
#ifndef NMEMSTATS
	 if(size) {
	    size += sizeof(size_t);
	    void* p = malloc(size);
#ifndef NMEMOPS
	    add_op(true, p, (byte_t*)p + size);
#endif
	    ((size_t*)p)[0] = size;
	    return (byte_t*)p + sizeof(size_t);
	 }
	 else
	    return NULL;
#else
	 if(size) {
#ifndef NMEMOPS
	    void* p = malloc(size);
	    add_op(true, p, (byte_t*)p + size);
	    return p;
#else
	    return malloc(size);
#endif
	 }
	 else
	    return NULL;
#endif
#else
	 void* p;
	 if(size) {
#ifndef NMEMSTATS
	    size += sizeof(size_t);
#endif
	    p = malloc(size + ptr_cookie_size);
	    ASSERT(p);
#ifndef NMEMOPS
	    add_op(true, p, (byte_t*)p + size + ptr_cookie_size);
#endif
	    memset(p, ptr_cookie_byte, ptr_cookie_size);
	    p = (void*)((byte_t*)p + ptr_cookie_size);
#ifndef NMEMSTATS
	    ((size_t*)p)[0] = size;
	    p = (byte_t*)p + sizeof(size_t);
#endif
	 }
	 else
	    p = NULL;
	 return p;
#endif
      }

      void
      new_free( void* ptr )
      {
	 if(ptr) {
	    CHECK(check_ptr_cookie(ptr));
	    void *p = ptr;
#ifndef NMEMSTATS
	    p = ((size_t*)p) - 1;
	    for(int ii = 0; ii < memory::ctx.num_selected(); ++ii) {
	       memory::state_t& state = memory::ctx.selected_group(ii).data();
	       state.size -= ((size_t*)p)[0];
	    }
#endif
#ifndef NDEBUG
	    p = (void*)((byte_t*)p - ptr_cookie_size);
	    memset(p, 0, ptr_cookie_size);
	    free(p);
#ifndef NMEMOPS
	    add_op(false, p, NULL);
#endif
#else
#ifndef NMEMOPS
	    add_op(false, ptr, NULL);
#endif
	    free(ptr);
#endif
	 }
      }

#ifndef NDEBUG

      void
      check_ptr_cookie( void* ptr )
      {
	 if(ptr) {
	    byte_t *p = (byte_t*)ptr - ptr_cookie_size;
#ifndef NMEMSTATS
	    p -= sizeof(size_t);
#endif
	    for(int ii = 0; ii < ptr_cookie_size; ii++)
	       ASSERT(p[ii] == ptr_cookie_byte);
	 }
      }

#endif

   }
}
