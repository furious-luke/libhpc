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
#include "libhpc/numerics/kde.hh"
#include "libhpc/containers/random.hh"
#include "libhpc/system/stream_output.hh"

using namespace hpc::test;

namespace {

   test_case<> ANON(
      "/libhpc/numerics/kde/constant",
      "",
      []()
      {
         typedef hpc::numerics::kde<
            double,
            hpc::numerics::bandwidths::constant<double>,
            hpc::numerics::kernels::epanechnikov<double>
            > kde_type;

         std::vector<double> pnts( 1000 );
         std::generate( pnts.begin(), pnts.end(), hpc::normal_generator<double>() );
         kde_type kde( pnts );
         kde.bandwidth().set_iqr( kde.points() );

         // std::cout << "\n";
         // for( unsigned ii = 0; ii < 100; ++ii )
         // {
         //    double x = -5.0 + (double)ii*(10.0/99.0);
         //    std::cout << x << ", " << kde.eval( x ) << "\n";
         // }
      }
      );

}
