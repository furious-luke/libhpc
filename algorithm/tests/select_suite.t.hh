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

#include <cxxtest/TestSuite.h>
#include "libhpc/algorithm/select.hh"

using namespace hpc;

class select_suite : public CxxTest::TestSuite {
public:

   void test_minimum()
   {
      vector<double> values( 10 );
      hpc::iota( values.begin(), values.end(), 10*mpi::comm::world.rank() );
      double x = algorithm::select( values.begin(), values.end(), 0 );
      TS_ASSERT_DELTA( x, 0.0, 1e-4 );
   }

   void test_maximum()
   {
      vector<double> values( 10 );
      hpc::iota( values.begin(), values.end(), 10*mpi::comm::world.rank() );
      double x = algorithm::select( values.begin(), values.end(), 10*mpi::comm::world.size() - 1 );
      TS_ASSERT_DELTA( x, (double)(10*mpi::comm::world.size() - 1), 1e-4 );
   }

   void test_7th()
   {
      vector<double> values( 10 );
      hpc::iota( values.begin(), values.end(), 10*mpi::comm::world.rank() );
      double x = algorithm::select( values.begin(), values.end(), 7 );
      TS_ASSERT( x >= 6.0 && x <= 7.0 );
   }
};
