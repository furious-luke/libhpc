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
#include "libhpc/regexp/multimatch.hh"

using namespace hpc;

class multimatch_suite : public CxxTest::TestSuite {
public:

   void test_empty()
   {
      multimatch mm;
      mm.compile();
      optional<hpc::index> match = mm( "hello world" );
      TS_ASSERT( !match );
   }

   void test_single_false()
   {
      multimatch mm;
      mm.add_match( "hello" );
      mm.compile();
      optional<hpc::index> match = mm( "world" );
      TS_ASSERT( !match );
   }

   void test_single_true()
   {
      multimatch mm;
      mm.add_match( "hello" );
      mm.compile();
      optional<hpc::index> match = mm( "hello" );
      TS_ASSERT( match );
   }

   void test_single_extra()
   {
      multimatch mm;
      mm.add_match( "hello" );
      mm.compile();
      optional<hpc::index> match = mm( "hellos" );
      TS_ASSERT( !match );
   }

   void test_multi_true()
   {
      multimatch mm;
      mm.add_match( "hello" );
      mm.add_match( "world" );
      mm.add_match( "zinga" );
      mm.compile();
      optional<hpc::index> match = mm( "zinga" );
      TS_ASSERT( match );
   }

   void test_hidden()
   {
      multimatch mm;
      mm.add_match( "hello" );
      mm.add_match( "he" );
      mm.add_match( "zinga" );
      mm.compile();
      optional<hpc::index> match = mm( "he" );
      TS_ASSERT( match && *match == 1 );
      match = mm( "hello" );
      TS_ASSERT( match && *match == 0 );

      mm.clear();
      mm.add_match( "he" );
      mm.add_match( "hello" );
      mm.add_match( "zinga" );
      mm.compile();
      match = mm( "he" );
      TS_ASSERT( match && *match == 0 );
      match = mm( "hello" );
      TS_ASSERT( match && *match == 1 );
   }
};
