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

#include <unistd.h>
#include <libhpc/unit_test/main.hh>
#include <libhpc/system/timer.hh>

TEST_CASE( "/libhpc/system/timer/stop" )
{
   hpc::timer<> timer;
   for( unsigned ii = 0; ii < 10; ++ii )
   {
      hpc::timer<>::handle hnd = timer.start();
      usleep( 100000 ); // sleep for 1/10 second
   }
   DELTA( timer.total().count(), 1.0, 1e-2 );
}

TEST_CASE( "/libhpc/system/timer/tally" )
{
   hpc::timer<> timer;
   for( unsigned ii = 0; ii < 10; ++ii )
   {
      hpc::timer<>::handle hnd = timer.start( hpc::timer<>::handle::TALLY );
      usleep( 100000 ); // sleep for 1/10 second
   }
   DELTA( timer.total().count(), 1.0, 1e-2 );
   DELTA( timer.mean().count(), 0.1, 1e-2 );
}
