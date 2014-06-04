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
#include <libhpc/algorithm/newton.hh>

struct parabola
{
   CUDA_DEV_HOST
   double
   operator()( double x ) const
   {
      return x*x - 16.0;
   }

   CUDA_DEV_HOST
   double
   deriv( double x,
          double f ) const
   {
      return 2.0*x;
   }
};

TEST_CASE_CUDA( "/hpc/algorithm/newton" )
{
   double x = hpc::alg::newton( parabola(), 3.0, 6.0, 5.5 );
   DELTA( x, 4.0, 1e-4 );
}
