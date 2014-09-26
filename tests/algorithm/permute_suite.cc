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

#include <boost/range/algorithm_ext/iota.hpp>
#include <libhpc/unit_test/main_mpi.hh>
#include <libhpc/algorithm/permute.hh>

typedef hpc::mpi::comm comm;

SUITE_PREFIX( "/hpc/algorithm/permute/" );

TEST_CASE( "basic" )
{
   std::vector<int> vals( 10 ), idxs( 10 );
   boost::iota( vals, 0 );
   idxs[0] = 3; idxs[3] = 8; idxs[8] = 2; idxs[2] = 0;
   idxs[1] = 4; idxs[4] = 1;
   idxs[5] = 5;
   idxs[6] = 7; idxs[7] = 9; idxs[9] = 6;
   hpc::permute( vals.begin(), vals.end(), idxs.begin() );
   int results[] = { 2, 4, 8, 0, 1, 5, 9, 6, 3, 7 };
   for( unsigned ii = 0; ii < 10; ++ii )
      TEST( vals[ii] == results[ii] );
}
