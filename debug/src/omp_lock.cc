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

#include "omp_lock.hh"

#ifdef _OPENMP

namespace hpc {
   namespace memory {

      omp_lock::omp_lock()
      {
	 omp_init_lock( &_lock );
      }

      omp_lock::~omp_lock()
      {
	 omp_destroy_lock( &_lock );
      }

      void
      omp_lock::set()
      {
	 omp_set_lock( &_lock );
      }

      void
      omp_lock::unset()
      {
	 omp_unset_lock( &_lock );
      }
   }
}

#endif
