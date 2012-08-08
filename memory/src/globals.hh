#ifndef memory_globals_hh
#define memory_globals_hh

namespace hpc {
   namespace memory {

#ifndef NMEMSTATS

      void
      select( const char* path );

      void
      deselect( const char* path );

      void
      set_log_file( const char* path );

      void
      log( const char* filename=NULL );

#endif

   }
}

#endif
