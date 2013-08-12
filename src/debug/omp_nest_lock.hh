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

#ifndef libhpc_debug_omp_nest_lock_hh
#define libhpc_debug_omp_nest_lock_hh

#ifdef _OPENMP

#include <omp.h>
#include "omp_lock.hh"

#define OMP_NEST_LOCK( name )			\
   ::hpc::memory::omp_nest_lock name;

namespace hpc {
   namespace memory {

      class omp_nest_lock
      {
      public:

	 omp_nest_lock();

	 ~omp_nest_lock();

	 void
	 set();

	 void
	 unset();

      protected:

	 omp_nest_lock_t _lock;
      };
   }
}

#else

#define OMP_NEST_LOCK( name )

#endif

#endif
