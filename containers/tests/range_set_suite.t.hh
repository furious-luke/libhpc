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

#include <numeric>
#include <cxxtest/TestSuite.h>
#include "libhpc/containers/range_set.hh"

using namespace hpc;

class range_set_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      range_set<int> set;
      TS_ASSERT_EQUALS( set.size(), 0 );
      TS_ASSERT_EQUALS( set.empty(), true );
   }

   void test_insert_covered()
   {
      range_set<int> set;
      set.insert( range<int>( 2, 8 ) );
      set.insert( range<int>( 0, 10 ) );
   }

   void test_insert_internal()
   {
      range_set<int> set;
      set.insert( range<int>( 0, 10 ) );
      set.insert( range<int>( 2, 8 ) );
   }

   void test_insert_equal()
   {
      range_set<int> set;
      set.insert( range<int>( 2, 8 ) );
      set.insert( range<int>( 2, 8 ) );
   }

   void test_insert_lower_equal()
   {
      range_set<int> set;
      set.insert( range<int>( 2, 8 ) );
      set.insert( range<int>( 2, 10 ) );
   }

   void test_insert_upper_equal()
   {
      range_set<int> set;
      set.insert( range<int>( 2, 8 ) );
      set.insert( range<int>( 0, 8 ) );
   }

   void test_insert_upper_bound()
   {
      range_set<int> set;
      set.insert( range<int>( 0, 20 ) );
      set.insert( range<int>( 0, 5 ) );
      set.insert( range<int>( 5, 10 ) );
      set.insert( range<int>( 10, 15 ) );
      set.insert( range<int>( 20, 25 ) );
      set.insert( range<int>( 8, 18 ) );
   }

   void setUp()
   {
   }
};
