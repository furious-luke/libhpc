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

#include <string>
#include <fstream>
// #ifndef MPICH_SKIP_MPICXX
// #define MPICH_SKIP_MPICXX
// #endif
// #ifndef OMPI_SKIP_MPICXX
// #define OMPI_SKIP_MPICXX
// #endif
#include <mpi.h>
#include "libhpc/debug/debug.hh"
#include "group_context.hh"
#include "state.hh"

#ifndef NMEMDEBUG

namespace hpc {
   namespace memory {

      OMP_LOCK( ctx_lock );

#ifndef NMEMSTATS

      memory::group_context<memory::state_t> ctx( true );
      std::string log_filename = "memory.stats";

      void
      select( const char* path )
      {
	 ctx.select( path );
      }

      void
      deselect( const char* path )
      {
	 ctx.deselect( path );
      }

      void
      set_log_filename( const char* filename )
      {
	 log_filename = filename;
      }

      void
      log( const char* filename )
      {
	 // Don't log anything unless we are rank 0.
	 // TODO: Fix this.
	 int flag;
	 MPI_Initialized( &flag );
	 if( flag )
	 {
	    int my_rank;
	    MPI_Comm_rank( MPI_COMM_WORLD, &my_rank );
	    if( my_rank != 0 )
	       return;
	 }

	 // Only dump the information if we are the master thread.
#pragma omp master
	 {
	    // Select either a passed in filename or the default.
	    std::string fn;
	    if( filename )
	       fn = filename;
	    else
	       fn = log_filename;

	    // Open the file and dump each group.
	    std::ofstream file( fn.c_str(), std::ios::out | std::ios::app );
	    ASSERT( (bool)file, "Error opening memory log file." );
	    for( auto& grp : memory::ctx )
	       file << grp.path() << " " << grp.data().size << " " << grp.data().peak << "\n";
	 }
      }

#endif
    
   }
}

#endif
