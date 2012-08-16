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
#include "libhpc/containers/range_multimap.hh"

using namespace hpc;

class range_multimap_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      range_multimap<int, int> map;
      TS_ASSERT_EQUALS( map.size(), 0 );
      TS_ASSERT_EQUALS( map.empty(), true );
   }

   void test_insert_upper_bound()
   {
      range_multimap<int, int> map;
      map.insert( range<int>( 0, 20 ), 0 );
      map.insert( range<int>( 0, 5 ), 1 );
      map.insert( range<int>( 5, 10 ), 2 );
      map.insert( range<int>( 10, 15 ), 3 );
      map.insert( range<int>( 20, 25 ), 4 );
      map.insert( range<int>( 8, 18 ), 5 );
   }

   void test_invert()
   {
      range_multimap<int, int> mapping;
      mapping.insert( range<int>( 0, 20 ), 0 );
      mapping.insert( range<int>( 0, 5 ), 1 );
      mapping.insert( range<int>( 5, 10 ), 2 );
      mapping.insert( range<int>( 10, 15 ), 3 );
      mapping.insert( range<int>( 20, 25 ), 4 );
      mapping.insert( range<int>( 8, 18 ), 5 );

      map<int, set<range<int>>> inv;
      invert_mapping( mapping, inv );
   }

   void test_medium()
   {
      range_multimap<int, int> mapping;
      for( unsigned ii = 0; ii < 200; ++ii )
         mapping.insert( range<int>( 0, 10*(ii + 1) ), ii );

      map<int, set<range<int>>> inv;
      invert_mapping( mapping, inv );
   }

   void setUp()
   {
   }
};
