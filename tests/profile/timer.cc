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

#include <string.h>
#include <unistd.h>
#include <libhpc/debug/unit_test_main.hh>
#include "libhpc/profile/timer.hh"

using namespace hpc;
using namespace hpc::test;

test_case<> ANON(
   "/profile/timer/stop",
   "",
   []()
   {
      profile::timer timer;
      for( unsigned ii = 0; ii < 10; ++ii )
      {
         auto hnd = timer.start();
	 usleep( 100000 ); // sleep for 1/10 second
      }
      DELTA( timer.total(), 1.0, 1e-2 );
   }
   );

test_case<> ANON(
   "/profile/timer/tally",
   "",
   []()
   {
      profile::timer timer;
      for( unsigned ii = 0; ii < 10; ++ii )
      {
	 auto hnd = timer.start( profile::timer::handle::TALLY );
	 usleep( 100000 ); // sleep for 1/10 second
      }
      DELTA( timer.total(), 1.0, 1e-2 );
      DELTA( timer.mean(), 0.1, 1e-2 );
   }
   );
