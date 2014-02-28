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
#include "libhpc/numerics/histogram.hh"

using namespace hpc::test;

namespace {

   test_case<> ANON(
      "/libhpc/numerics/histogram",
      "",
      []()
      {
         hpc::numerics::histogram<double> hist;
         hist.resize( 3, { 0.0, 3.0 } );
         TEST( hist.bins()[0] == 0 );
         TEST( hist.bins()[1] == 0 );
         TEST( hist.bins()[2] == 0 );
         hist.add_sample( 0.5 );
         TEST( hist.bins()[0] == 1 );
         TEST( hist.bins()[1] == 0 );
         TEST( hist.bins()[2] == 0 );
         hist.add_sample( 0.5 );
         TEST( hist.bins()[0] == 2 );
         TEST( hist.bins()[1] == 0 );
         TEST( hist.bins()[2] == 0 );
         hist.add_sample( 1.5 );
         TEST( hist.bins()[0] == 2 );
         TEST( hist.bins()[1] == 1 );
         TEST( hist.bins()[2] == 0 );
         hist.add_sample( 2.5 );
         TEST( hist.bins()[0] == 2 );
         TEST( hist.bins()[1] == 1 );
         TEST( hist.bins()[2] == 1 );
      }
      );

   test_case<> ANON(
      "/libhpc/numerics/histogram/shared_axis",
      "",
      []()
      {
         hpc::numerics::histogram<double> hist;
         hist.resize( 3, { 0.0, 3.0 } );
         hpc::numerics::histogram<double> dep( hist.axis() );
         TEST( hist.bins()[0] == 0 );
         TEST( hist.bins()[1] == 0 );
         TEST( hist.bins()[2] == 0 );
         hist.add_sample( 0.5 );
         TEST( hist.bins()[0] == 1 );
         TEST( hist.bins()[1] == 0 );
         TEST( hist.bins()[2] == 0 );
         TEST( dep.bins()[0] == 0 );
         TEST( dep.bins()[1] == 0 );
         TEST( dep.bins()[2] == 0 );
         dep.add_sample( 2.5 );
         TEST( dep.bins()[0] == 0 );
         TEST( dep.bins()[1] == 0 );
         TEST( dep.bins()[2] == 1 );
         TEST( hist.bins()[0] == 1 );
         TEST( hist.bins()[1] == 0 );
         TEST( hist.bins()[2] == 0 );
      }
      );

}
