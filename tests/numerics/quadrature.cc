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

#include "libhpc/debug/unit_test_main.hh"
#include "libhpc/numerics/quadrature.hh"

using namespace hpc;
using namespace hpc::test;

namespace {

   test_case<> ANON(
      "/numerics/quadrature/gauss_jacobi",
      "",
      []()
      {
         unsigned np = 3;
         vector<double> points( np );
         vector<double> weights( np );
         numerics::gauss_jacobi<double>( np, 0.0, 0.0, points.begin(), weights.begin() );
         DELTA( points[0], -0.774597, 1e-6 );
         DELTA( points[1], 0.0, 1e-6 );
         DELTA( points[2], 0.774597, 1e-6 );
         DELTA( weights[0], 0.555556, 1e-6 );
         DELTA( weights[1], 0.888889, 1e-6 );
         DELTA( weights[2], 0.555556, 1e-6 );

         np = 4;
         points.resize( np );
         weights.resize( np );
         numerics::gauss_jacobi<double>( np, 0.0, 0.0, points.begin(), weights.begin() );
         DELTA( points[0], -0.861136, 1e-6 );
         DELTA( points[1], -0.339981, 1e-6 );
         DELTA( points[2], 0.339981, 1e-6 );
         DELTA( points[3], 0.861136, 1e-6 );
         DELTA( weights[0], 0.347855, 1e-6 );
         DELTA( weights[1], 0.652145, 1e-6 );
         DELTA( weights[2], 0.652145, 1e-6 );
         DELTA( weights[3], 0.347855, 1e-6 );

         np = 5;
         points.resize( np );
         weights.resize( np );
         numerics::gauss_jacobi<double>( np, 0.0, 0.0, points.begin(), weights.begin() );
         DELTA( points[0], -0.906180, 1e-6 );
         DELTA( points[1], -0.538469, 1e-6 );
         DELTA( points[2], 0.0, 1e-6 );
         DELTA( points[3], 0.538469, 1e-6 );
         DELTA( points[4], 0.906180, 1e-6 );
         DELTA( weights[0], 0.236927, 1e-6 );
         DELTA( weights[1], 0.478629, 1e-6 );
         DELTA( weights[2], 0.568889, 1e-6 );
         DELTA( weights[3], 0.478629, 1e-6 );
         DELTA( weights[4], 0.236927, 1e-6 );
      }
      );

   // test_case<> ANON(
   //    "/numerics/quadrature/gauss_lobatto",
   //    "",
   //    []()
   //    {
   //       unsigned np = 8;
   //       vector<double> points( np );
   //       vector<double> weights( np );
   //       numerics::gauss_lobatto<double>( np, 0.0, 0.0, points.begin(), weights.begin() );
   //    }
   //    );

      // void test_outer_product()
      // {
      //    unsigned np = 16;
      //    vector<double> points_1d( np );
      //    vector<double> weights_1d( np );
      //    numerics::gauss_lobatto<double>( 0.0, 0.0, points_1d, weights_1d );

      //    fibre<double> points( 2, np*np );
      //    vector<double> weights( np*np );
      //    numerics::outer_product( points_1d, weights_1d, points, weights );
      // }

      // void test_quadrature()
      // {
      //    unsigned np = 16;
      //    fibre<double> points( 2, np*np );
      //    vector<double> weights( np*np );
      //    numerics::quadrature( numerics::gauss_lobatto_gen<double>(), np - 1, points, weights );
      // }

}
