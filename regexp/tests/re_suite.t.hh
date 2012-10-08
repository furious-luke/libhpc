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
#include "libhpc/regexp/re.hh"

using namespace hpc;

class dfa_suite : public CxxTest::TestSuite {
public:

   void test_simple()
   {
      re::re prog( "(a)|(b)|(c)" );
      TS_ASSERT( prog.match( "c" ) );
   }

   void test_match()
   {
      re::re re( "(o(n)e)|(two)|(t(h(r)e)e)" );
      TS_ASSERT( re.match( "one" ) );
      TS_ASSERT( re.match( "two" ) );
      TS_ASSERT( re.match( "three" ) );
      TS_ASSERT( !re.match( "hello" ) );
      TS_ASSERT( !re.match( "oneo" ) );
      TS_ASSERT( !re.match( "onethree" ) );
   }

   void test_match_last_capture()
   {
      re::match match;
      re::re re( "(one)|(two)|(three)" );
      re.match( "one", match );
      TS_ASSERT_EQUALS( match.last_capture(), 0 );
      re.match( "two", match );
      TS_ASSERT_EQUALS( match.last_capture(), 1 );
      re.match( "three", match );
      TS_ASSERT_EQUALS( match.last_capture(), 2 );
   }
};
