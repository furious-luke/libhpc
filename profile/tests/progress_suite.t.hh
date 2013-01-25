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
#include <cxxtest/TestSuite.h>
#include "libhpc/profile/progress.hh"

using namespace hpc;

class progress_suite : public CxxTest::TestSuite {
public:

   void test_set_local_size()
   {
      profile::progress prog;
      prog.set_local_size( 100 );
      TS_ASSERT_EQUALS( prog._lsize, 100.0 );
      TS_ASSERT_DELTA( prog._gsize, 100.0*mpi::comm::world.size(), 0.01 );
      TS_ASSERT_DELTA( prog._gsize_inv, 1.0/(100.0*mpi::comm::world.size()), 0.01 );
      TS_ASSERT_EQUALS( prog._comp, 0.0 );
      TS_ASSERT_EQUALS( prog._old_comp, 0.0 );
      TS_ASSERT_EQUALS( prog._gcomp, 0.0 );
   }

   void test_update()
   {
      profile::progress prog;
      prog.set_local_size( 100 );
      for( unsigned ii = 0; ii < 100; ++ii )
      {
	 prog.set_local_complete( ii + 1 );
	 prog.update();
	 if( mpi::comm::world.rank() == 0 )
	    std::cout << "\n" << prog.complete();
      }
      while( prog.test() )
      {
	 prog.update();
	 if( mpi::comm::world.rank() == 0 )
	    std::cout << "\n" << prog.complete();
      }
      if( mpi::comm::world.rank() == 0 )
	 std::cout << "\n" << prog._gcomp << "\n";
   }
};
