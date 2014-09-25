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
#include <libhpc/mpi/surjection.hh>
#include <libhpc/mpi/scatter.hh>

typedef hpc::mpi::comm comm;

SUITE_PREFIX( "/hpc/mpi/scatter/" );

TEST_CASE( "constructor" )
{
   std::vector<unsigned> idxs( 10 );
   for( unsigned ii = 0; ii < 10; ++ii )
      idxs[ii] = (10*comm::world.rank() + ii + 5)%(10*comm::world.size());
   hpc::mpi::scatter sc( 10*comm::world.size(), idxs, hpc::mpi::datatype::integer );

   std::vector<int> src( 10 ), dst( 10 );
   boost::iota( src, 10*comm::world.rank() );
   boost::iota( dst, 0 );

   sc.transfer( src, dst );
   for( unsigned ii = 0; ii < 10; ++ii )
      TEST( dst[ii] == (10*comm::world.rank() + ii + 5)%(10*comm::world.size()) );
}
