#ifndef libhpc_memory_frag_hh
#define libhpc_memory_frag_hh

namespace hpc {
   namespace memory {

#ifndef NMEMOPS

      void
      add_op( bool alloc,
	      const void* begin,
	      const void* end );

      void
      log_ops();

#endif

   }
}

#endif
