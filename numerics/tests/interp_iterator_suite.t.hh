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

#include <cxxtest/TestSuite.h>
#include <cxxtest/GlobalFixture.h>
#include "libhpc/numerics/interp_iterator.hh"

using namespace hpc;

class interp_iterator_suite : public CxxTest::TestSuite
{
public:

   void test_both_empty()
   {
      vector<float> grid0, grid1;
      interp_iterator<vector<float>::const_iterator> it = make_interp_iterator(
         grid0.cbegin(), grid0.cend(),
         grid1.cbegin(), grid1.cend()
         );
      bool inside = false;
      for( ; !it.done(); ++it )
         inside = true;
      TS_ASSERT( !inside );
   }

   void test_0_empty()
   {
      vector<float> grid0, grid1;
      setup_0( grid0 );
      interp_iterator<vector<float>::const_iterator> it = make_interp_iterator(
         grid0.cbegin(), grid0.cend(),
         grid1.cbegin(), grid1.cend()
         );
      unsigned ii = 0;
      for( ; !it.done(); ++it, ++ii )
      {
         TS_ASSERT_EQUALS( *it, (float)ii );
      }
   }

   void test_1_empty()
   {
      vector<float> grid0, grid1;
      setup_1( grid1 );
      interp_iterator<vector<float>::const_iterator> it = make_interp_iterator(
         grid0.cbegin(), grid0.cend(),
         grid1.cbegin(), grid1.cend()
         );
      unsigned ii = 0;
      for( ; !it.done(); ++it, ++ii )
      {
         TS_ASSERT_EQUALS( *it, -2.5 + (float)ii );
      }
   }

   void test_both()
   {
      vector<float> grid0, grid1;
      setup_0( grid0 );
      setup_1( grid1 );
      interp_iterator<vector<float>::const_iterator> it = make_interp_iterator(
         grid0.cbegin(), grid0.cend(),
         grid1.cbegin(), grid1.cend()
         );
      unsigned ii = 0;
      float results[15] = {-2.5, -1.5, -0.5, 0, 0.5, 1, 1.5, 2, 3, 4, 5, 6, 7, 8, 9};
      for( ; !it.done(); ++it, ++ii )
      {
         TS_ASSERT_EQUALS( *it, results[ii] );
      }
   }

   void test_overlapping()
   {
      vector<float> grid0, grid1;
      setup_0( grid0 );
      setup_1( grid1 );
      for( unsigned ii = 0; ii < grid1.size(); ++ii )
         grid1[ii] += 0.5;
      interp_iterator<vector<float>::const_iterator> it = make_interp_iterator(
         grid0.cbegin(), grid0.cend(),
         grid1.cbegin(), grid1.cend(),
         (float)1e-4
         );
      unsigned ii = 0;
      for( ; !it.done(); ++it, ++ii )
      {
         TS_ASSERT_EQUALS( *it, -2.0 + (float)ii );
      }
   }

   void setUp()
   {
      CLEAR_STACK_TRACE();
   }

   void setup_0( vector<float>& grid )
   {
      grid.resize( 10 );
      for( unsigned ii = 0; ii < 10; ++ii )
         grid[ii] = (float)ii;
   }

   void setup_1( vector<float>& grid )
   {
      grid.resize( 5 );
      for( unsigned ii = 0; ii < 5; ++ii )
         grid[ii] = -2.5 + (float)ii;
   }

private:

   int num_ranks, my_rank;
};
