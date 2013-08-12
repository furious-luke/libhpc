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
#include "libhpc/debug/group_context.hh"

using namespace hpc;

class group_context_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      debug::group_context<int> gc;
   }

   // void test_serial()
   // {
   //    debug::group_context<int> gc;
   //    gc.select( "/hello/world" );
   //    gc.select( "/face/palm" );
   //    TS_ASSERT_EQUALS( gc.num_selected(), 3 );
   //    for( unsigned ii = 0; ii < gc.num_selected(); ++ii )
   // 	 gc.selected_group( ii ).data() = ii;
   //    gc.select( "/hello/world" );
   //    gc.select( "/face/palm" );
   //    TS_ASSERT_EQUALS( gc.num_selected(), 1 );
   // }
};
