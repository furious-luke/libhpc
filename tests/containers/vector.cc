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

#include "libhpc/debug/unit_test.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/functors.hh"

using namespace hpc;
using namespace hpc::test;

struct fixture
{
   fixture()
   {
      vec.resize( 10 );
      hpc::iota( vec.begin(), vec.end(), 0 );
   }

   vector<int> vec;
};

void
check_empty( vector<int>& vec )
{
   TEST( vec.empty() == true, "Must be empty." );
}

void
check_contents( vector<int>& vec )
{
   TEST( vec.size() == 10 );
   for( int ii = 0; ii < 10; ++ii )
      TEST( vec[ii] == ii );
}

///
///
///
test_case<> ANON(
   "/containers/vector/default_constructor",
   "",
   []()
   {
      vector<int> vec;
      check_empty( vec );
   }
   );

///
///
///
test_case<fixture> ANON(
   "/containers/vector/view_conversion",
   "",
   []( fixture& fix )
   {
      vector<int>::view view = fix.vec;
   }
   );
