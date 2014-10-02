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

#ifndef libhpc_mpi_init_hh
#define libhpc_mpi_init_hh

// #ifndef MPICH_SKIP_MPICXX
// #define MPICH_SKIP_MPICXX
// #endif
//#ifndef OMPI_SKIP_MPICXX
//#define OMPI_SKIP_MPICXX
//#endif
#include <mpi.h>

namespace hpc {
   namespace mpi {

      void
      initialise();

      void
      initialise( int& argc,
                  char**& argv );

      bool
      initialised();

      void
      finalise( bool mpi = true );
   }
}

#endif
