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
#include <libhpc/unit_test/main.hh>
#include <libhpc/system/view.hh>

namespace {

   struct fixture
   {
      fixture()
         : vec( 10 )
      {
         std::iota( vec.begin(), vec.end(), 0 );
      }

      std::vector<int> vec;
   };

   void
   check_contents( hpc::view<std::vector<int>>& view,
                   size_t size = 10,
                   size_t offs = 0 )
   {
      TEST( view.size() == size );
      for( size_t ii = 0; ii < size; ++ii )
         TEST( view[ii] == offs + ii );
   }

   TEST_CASE( "/libhpc/system/view/constructor/one_to_one" )
   {
      fixture fix;
      hpc::view<std::vector<int>> view( fix.vec );
      check_contents( view );
   }

   TEST_CASE( "/libhpc/system/view/constructor/resize" )
   {
      fixture fix;
      hpc::view<std::vector<int>> view( fix.vec, 5 );
      check_contents( view, 5 );
   }

   TEST_CASE( "/libhpc/system/view/constructor/offset" )
   {
      fixture fix;
      hpc::view<std::vector<int>> view( fix.vec, 7, 3 );
      check_contents( view, 7, 3 );
   }

   TEST_CASE( "/libhpc/system/view/constructor/copy" )
   {
      fixture fix;
      hpc::view<std::vector<int>> view( fix.vec );
      hpc::view<std::vector<int>> copy( view );
      check_contents( view );
      check_contents( copy );
   }

   TEST_CASE( "/libhpc/system/view/size" )
   {
      fixture fix;
      hpc::view<std::vector<int>> view( fix.vec );
      TEST( view.size() == fix.vec.size() );
   }

}
