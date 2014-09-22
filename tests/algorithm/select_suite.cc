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
#include <libhpc/algorithm/select.hh>

typedef hpc::mpi::comm comm;

SUITE_PREFIX( "/hpc/algorithm/select" );

TEST_CASE( "minimum" )
{
   std::vector<double> values( 10 );
   boost::iota( values, 10*comm::world.rank() );
   double x = hpc::select( values.begin(), values.end(), 0 );
   TEST( x < 0.0 );
}

TEST_CASE( "maximum" )
{
   std::vector<double> values( 10 );
   boost::iota( values, 10*comm::world.rank() );
   double x = hpc::select( values.begin(), values.end(), 10*comm::world.size() );
   TEST( x >= (double)(10*comm::world.size() - 1) );
}

TEST_CASE( "7th" )
{
   std::vector<double> values( 10 );
   boost::iota( values, 10*comm::world.rank() );
   double x = hpc::select( values.begin(), values.end(), 7 );
   TEST( x >= 6.0 );
   TEST( x <= 7.0 );
}
