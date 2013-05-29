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
#include "libhpc/options/xml.hh"

using namespace hpc;

class dictionary_suite : public CxxTest::TestSuite {
public:

   void test_defaults()
   {
      options::dictionary dict;
      dict.add_option<options::integer>( "integer", "i", 10 );
      dict.compile();
      TS_ASSERT_EQUALS( dict.get<int>( "integer" ), 10 );
   }

   void test_bad_option()
   {
      options::dictionary dict;
      dict.compile();
      TS_ASSERT_THROWS( dict.get<int>( "integer" ), options::bad_option );
   }

   void test_no_value()
   {
      options::dictionary dict;
      dict.add_option<options::integer>( "integer", "i" );
      dict.compile();
      TS_ASSERT_THROWS( dict.get<int>( "integer" ), options::no_value );
   }

};
