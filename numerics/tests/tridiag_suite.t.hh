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
#include "libhpc/containers/num.hh"
#include "libhpc/numerics/tridiag.hh"

using namespace hpc;

class tridiag_suite : public CxxTest::TestSuite {
public:

   void test_symm_solve()
   {
      hpc::vector<double> diag( 3 ), off_diag( 2 ), rhs( 3 ), sol( 3 ), work( 2 );
      diag[0] = 2.0/1.0;
      diag[1] = 2.0*(1.0/1.0 + 1.0/3.0);
      diag[2] = 2.0/3.0;
      off_diag[0] = 1.0/1.0;
      off_diag[1] = 1.0/3.0;
      rhs[0] = 3.0*(-0.5/(1.0*1.0));
      rhs[1] = 3.0*(-0.5/(1.0*1.0) + 3.0/(3.0*3.0));
      rhs[2] = 3.0*(3.0/(3.0*3.0));
      numerics::tridiag_symm_solve<double>( diag, off_diag, rhs, sol, work );
      TS_ASSERT( num::approx<double>( sol[0], -0.6875, 0.00001 ) );
      TS_ASSERT( num::approx<double>( sol[1], -0.1250, 0.00001 ) );
      TS_ASSERT( num::approx<double>( sol[2], 1.5625, 0.00001 ) );
   }
};
