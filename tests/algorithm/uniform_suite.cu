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
#include <libhpc/algorithm/uniform.hh>
#include <libhpc/system/array.hh>

TEST_CASE_CUDA( "/hpc/algorithm/uniform/lift/3d" )
{
   hpc::array<int,3> sides = { 3, 3, 3 };
   for( int ii = 0; ii < 3*3*3; ++ii )
   {
      hpc::array<int,3> crds = hpc::lift<hpc::array<int,3>,3>( ii, sides );
      TEST_EQ( crds[2], ii/(sides[0]*sides[1]) );
      TEST_EQ( crds[1], (ii%(sides[0]*sides[1]))/sides[0] );
      TEST_EQ( crds[0], ii%sides[0] );
   }
}

TEST_CASE_CUDA( "/hpc/algorithm/uniform/project/3d" )
{
   hpc::array<int,3> sides = { 3, 3, 3 };
   for( int ii = 0; ii < sides[2]; ++ii )
   {
      for( int jj = 0; jj < sides[1]; ++jj )
      {
         for( int kk = 0; kk < sides[0]; ++kk )
         {
            hpc::array<int,3> crds = { ii, jj, kk };
	    int res = hpc::project<int,3>( crds, sides );
            TEST_EQ( res, sides[0]*(kk*sides[1] + jj) + ii );
         }
      }
   }
}
