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
#include "libhpc/options/xml_dict.hh"

using namespace hpc;

class xml_dict_suite : public CxxTest::TestSuite {
public:

   void test_merge()
   {
      string xml_str_1 = "<hello>hello</hello><integer>10</integer>";
      string xml_str_2 = "<world>world</world><integer>20</integer>";
      options::xml_dict dict;
      {
         std::stringstream ss( xml_str_1 );
         dict.read( ss );
      }
      {
         std::stringstream ss( xml_str_2 );
         dict.read( ss );
      }
      TS_ASSERT_EQUALS( dict.get<string>( "hello" ), "hello" );
      TS_ASSERT_EQUALS( dict.get<string>( "world" ), "world" );
      TS_ASSERT_EQUALS( dict.get<int>( "integer" ), 20 );
   }

   void test_list()
   {
      string xml_str = "<list><item>1</item><item>2</item></list>";
      options::xml_dict dict;
      {
         std::stringstream ss( xml_str );
         dict.read( ss );
      }
      list<int> val = dict.get_list<int>( "list" );
      TS_ASSERT_EQUALS( val.size(), 2 );
      auto it = val.begin();
      TS_ASSERT_EQUALS( *it, 1 );
      ++it;
      TS_ASSERT_EQUALS( *it, 2 );
   }

   void test_sub_options()
   {
      string xml_str = "<parent><child1>hello</child1><child2>10</child2></parent>";
      options::xml_dict dict;
      {
         std::stringstream ss( xml_str );
         dict.read( ss );
      }
      TS_ASSERT_EQUALS( dict.get<string>( "parent:child1" ), "hello" );
      TS_ASSERT_EQUALS( dict.get<int>( "parent:child2" ), 10 );
   }

};