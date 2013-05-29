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
#include "libhpc/options/cmd_line.hh"

using namespace hpc;

class cmd_line_suite : public CxxTest::TestSuite {
public:

   void test_empty()
   {
      int argc = 0;
      const char* argv[] = {};
      cl.parse( dict, argc, argv );
   }

   void test_long_names()
   {
      int argc = 3;
      const char* argv[] = { "--word=hello", "--integer=3", "--real=2.3" };
      cl.parse( dict, argc, argv );
      TS_ASSERT_EQUALS( dict.get<string>( "word" ), "hello" );
      TS_ASSERT_EQUALS( dict.get<int>( "integer" ), 3 );
      TS_ASSERT_DELTA( dict.get<float>( "real" ), 2.3, 0.01 );
   }

   void test_short_names()
   {
      int argc = 6;
      const char* argv[] = { "-w", "hello", "-i", "3", "-r", "2.3" };
      cl.parse( dict, argc, argv );
      TS_ASSERT_EQUALS( dict.get<string>( "word" ), "hello" );
      TS_ASSERT_EQUALS( dict.get<int>( "integer" ), 3 );
      TS_ASSERT_DELTA( dict.get<float>( "real" ), 2.3, 0.01 );
   }

   void test_positional()
   {
      int argc = 8;
      const char* argv[] = { "-w", "hello", "first", "-i", "3", "-r", "2.3", "second" };
      cl.parse( dict, argc, argv );
      TS_ASSERT_EQUALS( dict.get<string>( "word" ), "hello" );
      TS_ASSERT_EQUALS( dict.get<int>( "integer" ), 3 );
      TS_ASSERT_DELTA( dict.get<float>( "real" ), 2.3, 0.01 );
      TS_ASSERT_EQUALS( dict.get<string>( "pos1" ), "first" );
      TS_ASSERT_EQUALS( dict.get<string>( "pos2" ), "second" );
   }

   void setUp()
   {
      dict.add_option<options::string>( "word", "w" );
      dict.add_option<options::integer>( "integer", "i" );
      dict.add_option<options::real>( "real", "r" );
      dict.add_option<options::string>( "pos1", "*" );
      dict.add_option<options::string>( "pos2", "*" );
      dict.compile();
   }

   void tearDown()
   {
      dict.clear();
   }

   options::dictionary dict;
   options::cmd_line cl;
};
