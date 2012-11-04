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
#include "libhpc/memory/group_context.hh"

using namespace hpc;

class group_context_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      memory::group_context<int> gc;
   }

   void test_add_groups()
   {
      memory::group_context<int> gc;
      gc.select( "/hello/world" );
      gc.select( "/face/palm" );
      unsigned ii = 0;
      for( auto sel : gc )
	 ++ii;
      TS_ASSERT_EQUALS( ii, 3 );
   }

   void test_add_same_groups()
   {
      memory::group_context<int> gc;
      gc.select( "/hello/world" );
      gc.select( "/face/palm" );
      gc.select( "/hello/world" );
      gc.select( "/face/palm" );
      unsigned ii = 0;
      for( auto sel : gc )
	 ++ii;
      TS_ASSERT_EQUALS( ii, 3 );
   }

   void test_deselect_groups()
   {
      memory::group_context<int> gc;
      gc.select( "/hello/world" );
      gc.select( "/face/palm" );
      gc.deselect( "/hello/world" );
      gc.deselect( "/face/palm" );
      unsigned ii = 0;
      for( auto sel : gc )
	 ++ii;
      TS_ASSERT_EQUALS( ii, 1 );
   }

   void test_deselect_multi_groups()
   {
      memory::group_context<int> gc;
      gc.select( "/hello/world" );
      gc.select( "/face/palm" );
      gc.select( "/hello/world" );
      gc.select( "/face/palm" );
      gc.deselect( "/hello/world" );
      gc.deselect( "/face/palm" );
      unsigned ii = 0;
      for( auto sel : gc )
	 ++ii;
      TS_ASSERT_EQUALS( ii, 3 );
      gc.deselect( "/hello/world" );
      gc.deselect( "/face/palm" );
      ii = 0;
      for( auto sel : gc )
	 ++ii;
      TS_ASSERT_EQUALS( ii, 1 );
   }

   void test_threaded()
   {
      memory::group_context<int> gc;
#pragma parallel num_threads(3)
      {
	 if( OMP_TID == 0 )
	 {
	    gc.select( "/hello/world" );
	    unsigned ii = 0;
	    for( auto sel : gc )
	    {
	       TS_ASSERT( sel.path() == "/" || sel.path() == "/hello/world" );
	       ++ii;
	    }
	    TS_ASSERT_EQUALS( ii, 2 );
	 }
	 else if( OMP_TID == 1 )
	 {
	    gc.select( "/hello/world1" );
	    gc.select( "/face/palm1" );
	    unsigned ii = 0;
	    for( auto sel : gc )
	    {
	       TS_ASSERT( sel.path() == "/" || sel.path() == "/hello/world1" || sel.path() == "/face/palm1" );
	       ++ii;
	    }
	    TS_ASSERT_EQUALS( ii, 3 );
	 }
	 else
	 {
	    unsigned ii = 0;
	    for( auto sel : gc )
	    {
	       TS_ASSERT( sel.path() == "/" );
	       ++ii;
	    }
	    TS_ASSERT_EQUALS( ii, 1 );
	 }
      }
   }
};
