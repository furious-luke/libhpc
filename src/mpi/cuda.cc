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

#ifdef HAVE_CUDA

#include <cuda.h>
#include "cuda.hh"
#include "host.hh"

namespace hpc {
   namespace cuda {

      void
      select_device( mpi::comm const& comm )
      {
	 int n_devs;
	 INSIST( cudaGetDeviceCount( &n_devs ), == cudaSuccess );
	 EXCEPT( n_devs > 0, "No GPU capability on host: ", mpi::get_host() );
	 LOGILN( n_devs, " GPU(s) available on: ", mpi::get_host() );
	 if( n_devs == 1 )
	 {
	    LOGILN( "Rank ", comm.rank(), " on host ", mpi::get_host(), " using CUDA device: 0" );
	    auto res = cudaSetDevice( 0 );
	    EXCEPT( res == cudaSuccess, "Failed to set device." );
	 }
	 else if( n_devs > 1 )
	 {
	    std::set<int> ranks = mpi::make_host_ranks( comm );
	    LOGILN( "Ranks shared on host: ", ranks );
	    EXCEPT( ranks.size() <= n_devs, "Number of nodes on a host exceeds available GPUs." );
	    unsigned dev = 0;
	    for( auto rank : ranks )
	    {
	       if( comm.rank() == rank )
	       {
		  LOGILN( "Rank ", rank, " on host ", mpi::get_host(), " using CUDA device: ", dev );
		  auto res = cudaSetDevice( dev );
		  EXCEPT( res == cudaSuccess, "Failed to set device." );
	       }
	       ++dev;
	    }
	 }
      }

   }
}

#endif
