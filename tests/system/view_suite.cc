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
#include <numeric>
#include <boost/range/algorithm_ext/iota.hpp>
#include <libhpc/unit_test/main.hh>
#include <libhpc/system/view.hh>

struct fixture
{
   fixture()
      : vec( 10 )
   {
      boost::iota( vec, 0 );
   }

   std::vector<int> vec;
};

template< class View >
void
check_contents( hpc::test::test_case_base& tc,
                View const& view,
                size_t size = 10,
                size_t offs = 0 )
{
   TEST( view.size() == size );
   for( size_t ii = 0; ii < size; ++ii )
      TEST( view[ii] == offs + ii );
}

TEST_CASE( "/hpc/system/view/constructor/one_to_one" )
{
   fixture fix;
   hpc::view<std::vector<int> > view( fix.vec );
   check_contents( tc, view );
   hpc::view<std::vector<int> const> cv( fix.vec );
   check_contents( tc, cv );
}

TEST_CASE( "/hpc/system/view/constructor/resize" )
{
   fixture fix;
   hpc::view<std::vector<int> > view( fix.vec, 5 );
   check_contents( tc, view, 5 );
   hpc::view<std::vector<int> const> cv( fix.vec, 5 );
   check_contents( tc, cv, 5 );
}

TEST_CASE( "/hpc/system/view/constructor/offset" )
{
   fixture fix;
   hpc::view<std::vector<int> > view( fix.vec, 7, 3 );
   check_contents( tc, view, 7, 3 );
   hpc::view<std::vector<int> const> cv( fix.vec, 7, 3 );
   check_contents( tc, cv, 7, 3 );
}

TEST_CASE( "/hpc/system/view/constructor/copy" )
{
   fixture fix;
   hpc::view<std::vector<int> > view( fix.vec );
   hpc::view<std::vector<int> > copy( view );
   check_contents( tc, view );
   check_contents( tc, copy );
}

TEST_CASE( "/hpc/system/view/size" )
{
   fixture fix;
   hpc::view<std::vector<int> > view( fix.vec );
   TEST( view.size() == fix.vec.size() );
   hpc::view<std::vector<int> const> cv( fix.vec );
   TEST( cv.size() == fix.vec.size() );
}
