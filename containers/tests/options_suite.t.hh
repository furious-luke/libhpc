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
#include "libhpc/containers/options.hh"
#include "libhpc/containers/num.hh"

using namespace hpc;

class options_suite : public CxxTest::TestSuite {
public:

   void test_dictionary_insert()
   {
      options::dictionary dict;
      dict.insert<std::string>( "word", "chicken" );
      dict.insert<int>( "integer", 10 );
      dict.insert<float>( "real", 22.3 );
   }

   void test_dictionary_get()
   {
      options::dictionary dict;
      dict.insert<std::string>( "word", "chicken" );
      dict.insert<int>( "integer", 10 );
      dict.insert<float>( "real", 22.3 );

      TS_ASSERT_EQUALS( dict.get<std::string>( "word" ), std::string( "chicken" ) );
      TS_ASSERT_EQUALS( dict.get<int>( "integer" ), 10 );
      TS_ASSERT( num::approx<float>( dict.get<float>( "real" ), 22.3, 1e-4 ) );
   }
};
