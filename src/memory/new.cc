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
#include <memory>
#include "libhpc/debug/debug.hh"
#include "new.hh"
#include "state.hh"
#include "frag.hh"
#include "group_context.hh"

typedef unsigned char byte_t;

#ifndef NMEMDEBUG

#if CXX_0X

void*
operator new( size_t size )
{
   return hpc::memory::new_alloc(size);
}

void
operator delete( void* ptr ) noexcept
{
   hpc::memory::new_free(ptr);
}

void*
operator new[]( size_t size )
{
   return hpc::memory::new_alloc(size);
}

void
operator delete[]( void* ptr ) noexcept
{
   hpc::memory::new_free(ptr);
}

#else

void*
operator new( size_t size ) throw( std::bad_alloc )
{
   return hpc::memory::new_alloc(size);
}

void
operator delete( void* ptr ) throw()
{
   hpc::memory::new_free(ptr);
}

void*
operator new[]( size_t size ) throw( std::bad_alloc )
{
   return hpc::memory::new_alloc(size);
}

void
operator delete[]( void* ptr ) throw()
{
   hpc::memory::new_free(ptr);
}

#endif

namespace hpc {
   namespace memory {

      static const int ptr_cookie_size = 4;
      static const int ptr_cookie_byte = 0xef;

      bool stats_enabled = false;
      extern memory::group_context<memory::state_t> ctx;

#ifndef NMEMSTATS

      void
      enable_stats( bool state )
      {
	 stats_enabled = state;
      }

      void
      add_stats( size_t size )
      {
	 // Sometimes allocations may occur during C++ initialisation
	 // phases. This'll cause big problems if the memory group
	 // context isn't initialised first. So, switch it off until
	 // I make an explicit call.
	 if( stats_enabled )
	 {
#pragma omp critical( new_add_stats )
	    for( auto& grp : memory::ctx )
	    {
	       memory::state_t& state = grp.data();
	       state.size += size;
	       if( state.size > state.peak )
		  state.peak = state.size;
	    }
	 }
      }

      void
      rem_stats( size_t size )
      {
	 if( stats_enabled )
	 {
#pragma omp critical( new_rem_stats )
	 for( auto& grp : memory::ctx )
	 {
	    memory::state_t& state = grp.data();
	    ASSERT( size <= state.size,
		    "Uh-oh, some kind of internal memory book-keeping problem." );
	    state.size -= size;
	 }
	 }
      }

#endif

      void*
      new_alloc( size_t size )
      {
#ifdef NDEBUG
#ifndef NMEMSTATS
	 if( size )
	 {
	    size += sizeof(size_t);
	    void* p = malloc( size );
	    add_stats( size );
#ifndef NMEMOPS
	    add_op( true, p, (byte_t*)p + size );
#endif
	    ((size_t*)p)[0] = size;
	    return (byte_t*)p + sizeof(size_t);
	 }
	 else
	    return NULL;
#else
	 if( size )
	 {
#ifndef NMEMOPS
	    void* p = malloc( size );
	    add_op( true, p, (byte_t*)p + size );
	    return p;
#else
	    return malloc( size );
#endif
	 }
	 else
	    return NULL;
#endif
#else
	 void* p;
	 if( size )
	 {
	    size += ptr_cookie_size;
#ifndef NMEMSTATS
	    size += sizeof(size_t);
	    add_stats( size );
#endif
	    p = malloc( size );
            if( !p )
               throw std::bad_alloc();
#ifndef NMEMOPS
	    add_op( true, p, (byte_t*)p + size );
#endif
	    memset( p, ptr_cookie_byte, ptr_cookie_size );
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
	 if( ptr )
	 {
	    CHECK( check_ptr_cookie( ptr ) );
	    void* p = ptr;
#ifndef NMEMSTATS
	    p = ((size_t*)p) - 1;
	    rem_stats( ((size_t*)p)[0] );
#endif
#ifndef NDEBUG
	    p = (void*)((byte_t*)p - ptr_cookie_size);
	    memset( p, 0, ptr_cookie_size );
	    free( p );
#ifndef NMEMOPS
	    add_op( false, p, NULL );
#endif
#else
#ifndef NMEMOPS
	    add_op( false, ptr, NULL );
#endif
	    free( ptr );
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

#endif
