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

#include <vector>
#include <libhpc/unit_test/main.hh>
#include <libhpc/system/matrix.hh>
#include <libhpc/system/view.hh>

TEST_CASE( "/hpc/system/matrix/rows_iterator" )
{
   hpc::matrix<int> mat( 4, 3 );
   for( int ii = 0; ii < 4; ++ii )
   {
      for( int jj = 0; jj < 3; ++jj )
         mat( ii, jj ) = 3*ii + jj;
   }
   int ii = 0;
   for( hpc::matrix<int>::const_rows_iterator it = mat.rows_begin();
        it != mat.rows_end();
        ++it )
   {
      hpc::view<std::vector<int> const> row = *it;
      for( int jj = 0; jj < 3; ++jj )
         TEST( row[jj] == ii++ );
   }
}
