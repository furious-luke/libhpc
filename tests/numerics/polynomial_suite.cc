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

#include <libhpc/unit_test/main.hh>
#include <libhpc/numerics/polynomial.hh>

TEST_CASE( "/libhpc/numerics/polynomial/jacobi" )
{
   hpc::polynomial::jacobi<double> jacobi( 16, 1.0, 1.0 );
   for( unsigned ii = 0; ii < 100; ++ii )
   {
      double x = -1.0 + ((double)ii)*(2.0/99.0);
      double f, df;
      f = jacobi.derivative( x, df );
   }
}
