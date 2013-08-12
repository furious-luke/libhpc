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
#include "libhpc/containers/range_map.hh"

using namespace hpc;

class range_map_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      // range_map<int,int> map;
      // TS_ASSERT_EQUALS( map.size(), 0 );
      // TS_ASSERT_EQUALS( map.empty(), true );
   }

   void test_insert_one()
   {
      // range_map<int,int> map;
      // for( auto it = map.insert( range<int>( 0, 10 ), 1 ); !it.done(); ++it )
      // {
      //    TS_ASSERT_EQUALS( it->first, range<int>( 0, 10 ) );
      //    TS_ASSERT_EQUALS( it->second, 1 );
      // }
      // TS_ASSERT_EQUALS( map.size(), 1 );
      // TS_ASSERT_EQUALS( map.empty(), false );
   }
};
