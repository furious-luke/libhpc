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

#include <array>
#include "libhpc/debug/unit_test_main.hh"
#include "libhpc/numerics/least_squares.hh"

using namespace hpc::test;

namespace {

   test_case<> ANON(
      "/libhpc/numerics/least_squares/1d/linear",
      "",
      []()
      {
         hpc::numerics::least_squares<double> ls;
         ls.resize( 10 );
         for( int ii = 0; ii < 10; ++ii )
            ls.add_sample( (double)ii, (double)(ii + 1) );
         ls.solve();
         DELTA( ls.solution()( 0 ), 1.0, 1e-4 );
         DELTA( ls.solution()( 1 ), 1.0, 1e-4 );

         ls.reset();
         for( int ii = 0; ii < 10; ++ii )
            ls.add_sample( (double)ii, (double)(-ii - 1) );
         ls.solve();
         DELTA( ls.solution()( 0 ), -1.0, 1e-4 );
         DELTA( ls.solution()( 1 ), -1.0, 1e-4 );
      }
      );

   test_case<> ANON(
      "/libhpc/numerics/least_squares/1d/quadratic",
      "",
      []()
      {
         hpc::numerics::least_squares<double> ls;
         ls.resize( 10, 2 );
         for( int ii = 0; ii < 10; ++ii )
            ls.add_sample( (double)ii, (double)(ii*ii) );
         ls.solve();
         DELTA( ls.solution()( 0 ), 0.0, 1e-4 );
         DELTA( ls.solution()( 1 ), 0.0, 1e-4 );
         DELTA( ls.solution()( 2 ), 1.0, 1e-4 );

         ls.reset();
         for( int ii = 0; ii < 10; ++ii )
            ls.add_sample( (double)ii, (double)(ii + 1)*(ii + 1) );
         ls.solve();
         DELTA( ls.solution()( 0 ), 1.0, 1e-4 );
         DELTA( ls.solution()( 1 ), 2.0, 1e-4 );
         DELTA( ls.solution()( 2 ), 1.0, 1e-4 );
      }
      );

   test_case<> ANON(
      "/libhpc/numerics/least_squares/2d/linear",
      "",
      []()
      {
         hpc::numerics::least_squares<double> ls;
         ls.resize( 100, 1, 2 );
         for( int ii = 0; ii < 10; ++ii )
         {
            for( int jj = 0; jj < 10; ++jj )
            {
               std::array<double,2> crd{ { (double)ii, (double)jj } };
               ls.add_sample( crd.begin(), (double)(1 + ii + 2*jj + ii*jj) );
            }
         }
         ls.solve();
         DELTA( ls.solution()( 0 ), 1.0, 1e-4 );
         DELTA( ls.solution()( 1 ), 1.0, 1e-4 );
         DELTA( ls.solution()( 2 ), 2.0, 1e-4 );
         DELTA( ls.solution()( 3 ), 1.0, 1e-4 );
      }
      );

   test_case<> ANON(
      "/libhpc/numerics/least_squares/2d/quadratic",
      "",
      []()
      {
         hpc::numerics::least_squares<double> ls;
         ls.resize( 100, 2, 2 );
         for( int ii = 0; ii < 10; ++ii )
         {
            for( int jj = 0; jj < 10; ++jj )
            {
               std::array<double,2> crd{ { (double)ii, (double)jj } };
               ls.add_sample( crd.begin(), (double)(1 + ii + 2*jj + ii*jj + ii*ii + 2*jj*jj + ii*ii*jj*jj) );
            }
         }
         ls.solve();
         DELTA( ls.solution()( 0 ), 1.0, 1e-4 );
         DELTA( ls.solution()( 1 ), 1.0, 1e-4 );
         DELTA( ls.solution()( 2 ), 1.0, 1e-4 );
         DELTA( ls.solution()( 3 ), 2.0, 1e-4 );
         DELTA( ls.solution()( 4 ), 1.0, 1e-4 );
         DELTA( ls.solution()( 5 ), 0.0, 1e-4 );
         DELTA( ls.solution()( 6 ), 2.0, 1e-4 );
         DELTA( ls.solution()( 7 ), 0.0, 1e-4 );
         DELTA( ls.solution()( 8 ), 1.0, 1e-4 );
      }
      );

}
