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

#ifndef libhpc_profile_timer_hh
#define libhpc_profile_timer_hh

#include "libhpc/system/timer.hh"
#include "libhpc/mpi/comm.hh"

namespace hpc {
   namespace profile {

      ///
      ///
      ///
      class timer
      {
      public:

	 timer();

	 ~timer();

	 void
	 reset();

	 bool
	 running() const;

	 void
	 start();

	 void
	 stop();

	 void
	 stop_tally();

	 unsigned long
	 count() const;

	 double
	 total() const;

	 double
	 total( const mpi::comm& comm ) const;

	 double
	 mean() const;

	 double
	 mean( const mpi::comm& comm ) const;

      protected:

	 time_type _start;
	 double _total;
	 unsigned long _cnt;
	 bool _run;
	 unsigned _stack;
      };

   }
}

#endif
