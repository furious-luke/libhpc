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
#include "libhpc/numerics/matrix.hh"
#include "libhpc/numerics/vector.hh"

using namespace hpc;

class matrix_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      numerics::matrix<double> mat;
   }

   void test_default_ctor_rows_cols()
   {
      numerics::matrix<double,3,3> mat;
   }

   void test_access()
   {
      numerics::matrix<double,3,3> mat;
      mat( 0, 0 ) = 1.0;
      mat( 1, 0 ) = 2.0;
      mat( 0, 1 ) = 3.0;
   }

   void test_solve_identity()
   {
      numerics::matrix<double,2,2> mat;
      mat(0,0) = 1.0;
      mat(1,0) = 0.0;
      mat(0,1) = 0.0;
      mat(1,1) = 1.0;
      numerics::vector<double,2> rhs, sol;
      rhs(0) = 2.0;
      rhs(1) = 3.0;
      mat.solve( rhs, sol );
   }

   void test_solve()
   {
      numerics::matrix<double,2,2> mat;
      mat(0,0) = 2.0;
      mat(1,0) = 1.0;
      mat(0,1) = 1.0;
      mat(1,1) = 2.0;
      numerics::vector<double,2> rhs, sol;
      rhs(0) = 2.0;
      rhs(1) = 3.0;
      mat.solve( rhs, sol );
   }
};
