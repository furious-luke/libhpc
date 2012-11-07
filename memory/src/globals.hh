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

#ifndef libhpc_memory_globals_hh
#define libhpc_memory_globals_hh

#ifndef NMEMSTATS

#define MEM_SELECT( path )			\
   ::hpc::memory::select( path )

#define MEM_DESELECT( path )			\
   ::hpc::memory::deselect( path )

#else

#define MEM_SELECT( path )
#define MEM_DESELECT( path )

#endif

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
