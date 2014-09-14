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
#include <libhpc/algorithm/binary_partitioner.hh>

SUITE_PREFIX( "/hpc/algorithm/binary_partitioner/" );

TEST_CASE( "construct" )
{
   std::array<std::vector<double>,3> crds;
   for( unsigned ii = 0; ii < 3; ++ii )
   {
      for( unsigned jj = 0; jj < 10; ++jj )
	 crds[ii].push_back( 10*(ii + 1)*(jj + 1) );
   }
   hpc::binary_partitioner bp( crds.begin(), crds.end(), 1 );
}
