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

#include <libhpc/mpi/unit_test_main.hh>
#include "libhpc/algorithm/reorder.hh"
#include "libhpc/containers/random.hh"

using namespace hpc::test;

namespace {

   test_case<> ANON(
      "/libhpc/algorithm/reorder",
      "",
      []()
      {
	 std::vector<int> data( 10 ), order( 10 );
	 for( unsigned jj = 0; jj < 10; ++jj )
	 {
	    std::iota( data.begin(), data.end(), 0 );
	    std::iota( order.begin(), order.end(), 0 );
	    for( unsigned ii = 0; ii < 100; ++ii )
	    {
	       int a = hpc::generate_uniform<int>( 0, 9 );
	       int b = hpc::generate_uniform<int>( 0, 9 );
	       std::swap( order[a], order[b] );
	    }
	    hpc::reorder( data.begin(), data.end(), order.begin() );
	    for( unsigned ii = 0; ii < order.size(); ++ii )
	       TEST( data[order[ii]] == ii );
	 }
      }
      );

}
