#ifndef containers_new_hh
#define containers_new_hh

#include <stdlib.h>
#include <string.h>
#include <set>

#ifndef WITH_TAU

void* operator new(size_t size);
void operator delete(void* ptr);
void* operator new[](size_t size);
void operator delete[](void* ptr);

#endif

namespace hpc {
   namespace memory {

      void*
      new_alloc( size_t size );

      void
      new_free( void* ptr );

#if !defined NDEBUG && !defined WITH_TAU

      void check_ptr_cookie(void* ptr);

#endif

   }
}

#endif
