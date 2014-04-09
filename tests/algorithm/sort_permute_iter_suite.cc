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
#include <libhpc/algorithm/sort_permute_iter.hh>

TEST_CASE( "/libhpc/algorithm/sort_permute_iter" )
{
   std::vector<int> order( 10 );
   std::vector<float> data( 10 );
   for( unsigned ii = 0; ii < 10; ++ii )
      order[ii] = (ii + 4)%10;
   std::iota( data.begin(), data.end(), 0.0 );
   std::sort(
      hpc::make_sort_permute_iter( order.begin(), data.begin() ),
      hpc::make_sort_permute_iter( order.end(), data.end() ),
      hpc::sort_permute_iter_compare<std::vector<int>::iterator,std::vector<float>::iterator>()
      );
   for( unsigned ii = 0; ii < 10; ++ii )
   {
      TEST( order[ii] == ii );
      TEST( data[ii] == (float)((ii + 6)%10) );
   }
}
