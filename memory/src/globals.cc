#include <string>
#include <stdio.h>
#ifndef MPICH_SKIP_MPICXX
#define MPICH_SKIP_MPICXX
#endif
#ifndef OMPI_SKIP_MPICXX
#define OMPI_SKIP_MPICXX
#endif
#include <mpi.h>
#include "libhpc/debug/debug.hh"
#include "state.hh"

namespace hpc {
   namespace memory {

#ifndef NMEMSTATS

      debug::group_context<memory::state_t> ctx(true);
      std::string log_filename = "memory.stats";

      void
      select( const char* path )
      {
	 ctx.select(path);
      }

      void
      deselect( const char* path )
      {
	 ctx.deselect(path);
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
	 MPI_Initialized(&flag);
	 if(flag) {
	    int myRank;
	    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	    if(myRank != 0)
	       return;
	 }

	 std::string fn;
	 if(filename)
	    fn = filename;
	 else
	    fn = log_filename;

	 FILE* fp = fopen(fn.c_str(), "a");
	 ASSERT(fp, "Error opening memory log file.");
	 for(size_t ii = 0; ii < memory::ctx.num_groups(); ++ii) {
	    const debug::group<state_t>& grp = memory::ctx.group(ii);
	    fprintf(fp, "%s  %ld  %ld\n", grp.path(), grp.data().size, grp.data().peak);
	 }
	 fclose(fp);
      }

#endif
    
   }
}
