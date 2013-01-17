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

class xml_suite : public CxxTest::TestSuite {
public:

   void test_empty()
   {
      options::dictionary dict;
      dict.add_option( new options::string( "word" ) );
      dict.add_option( new options::integer( "integer" ) );
      dict.add_option( new options::real( "real" ) );
      dict.compile();

      options::xml xml;
      string filename = tmpnam( NULL );
      xml.write( filename, dict );
      remove( filename.c_str() );
   }

   void test_save_and_load()
   {
      options::dictionary dict;
      dict.add_option( new options::string( "word" ) );
      dict.add_option( new options::string( "yeah" ) );
      dict.add_option( new options::integer( "integer" ) );
      dict.add_option( new options::real( "real" ) );
      dict.compile();

      dict["word"] = "hello";
      dict["yeah"] = "world";
      dict["integer"] = "5";
      dict["real"] = "10.3";

      options::xml xml;
      string filename = tmpnam( NULL );
      xml.write( filename, dict );

      options::dictionary new_dict;
      new_dict.add_option( new options::string( "word" ) );
      new_dict.add_option( new options::string( "yeah" ) );
      new_dict.add_option( new options::integer( "integer" ) );
      new_dict.add_option( new options::real( "real" ) );
      new_dict.compile();

      xml.read( filename, new_dict );

      TS_ASSERT_EQUALS( dict.get<string>( "word" ), new_dict.get<string>( "word" ) );
      TS_ASSERT_EQUALS( dict.get<string>( "yeah" ), new_dict.get<string>( "yeah" ) );
      TS_ASSERT_EQUALS( dict.get<int>( "integer" ), new_dict.get<int>( "integer" ) );
      TS_ASSERT_EQUALS( dict.get<float>( "real" ), new_dict.get<float>( "real" ) );

      remove( filename.c_str() );
   }

   void test_sub_dictionary()
   {
      options::dictionary* dict1 = new options::dictionary( "sub" );
      dict1->add_option( new options::string( "word" ) );
      dict1->add_option( new options::integer( "integer" ) );
      dict1->add_option( new options::real( "real" ) );
      options::dictionary dict2;
      dict2.add_option( new options::string( "word" ) );
      dict2.add_option( new options::integer( "integer" ) );
      dict2.add_option( new options::real( "real" ) );
      dict2.add_dictionary( dict1 );
      dict2.compile();

      dict2["word"] = "hello";
      dict2["integer"] = "5";
      dict2["real"] = "10.3";
      dict2["sub:word"] = "world";
      dict2["sub:integer"] = "10";
      dict2["sub:real"] = "20.6";

      options::xml xml;
      string filename = tmpnam( NULL );
      xml.write( filename, dict2 );

      options::dictionary* new_dict1 = new options::dictionary( "sub" );
      new_dict1->add_option( new options::string( "word" ) );
      new_dict1->add_option( new options::integer( "integer" ) );
      new_dict1->add_option( new options::real( "real" ) );
      options::dictionary new_dict2;
      new_dict2.add_option( new options::string( "word" ) );
      new_dict2.add_option( new options::integer( "integer" ) );
      new_dict2.add_option( new options::real( "real" ) );
      new_dict2.add_dictionary( new_dict1 );
      new_dict2.compile();

      xml.read( filename, new_dict2 );

      TS_ASSERT_EQUALS( dict2.get<string>( "word" ), new_dict2.get<string>( "word" ) );
      TS_ASSERT_EQUALS( dict2.get<int>( "integer" ), new_dict2.get<int>( "integer" ) );
      TS_ASSERT_EQUALS( dict2.get<float>( "real" ), new_dict2.get<float>( "real" ) );
      TS_ASSERT_EQUALS( dict2.get<string>( "sub:word" ), new_dict2.get<string>( "sub:word" ) );
      TS_ASSERT_EQUALS( dict2.get<int>( "sub:integer" ), new_dict2.get<int>( "sub:integer" ) );
      TS_ASSERT_EQUALS( dict2.get<float>( "sub:real" ), new_dict2.get<float>( "sub:real" ) );

      remove( filename.c_str() );
   }

   void test_path()
   {
      options::dictionary* dict1 = new options::dictionary( "sub" );
      dict1->add_option( new options::string( "word" ) );
      dict1->add_option( new options::integer( "integer" ) );
      dict1->add_option( new options::real( "real" ) );
      options::dictionary dict2;
      dict2.add_option( new options::string( "word" ) );
      dict2.add_option( new options::integer( "integer" ) );
      dict2.add_option( new options::real( "real" ) );
      dict2.add_dictionary( dict1 );
      dict2.compile();

      dict2["word"] = "hello";
      dict2["integer"] = "5";
      dict2["real"] = "10.3";
      dict2["sub:word"] = "world";
      dict2["sub:integer"] = "10";
      dict2["sub:real"] = "20.6";

      options::xml xml;
      string filename = tmpnam( NULL );
      xml.write( filename, dict2 );

      options::dictionary new_dict1;
      new_dict1.add_option( new options::string( "word" ) );
      new_dict1.add_option( new options::integer( "integer" ) );
      new_dict1.add_option( new options::real( "real" ) );
      new_dict1.compile();

      xml.read( filename, new_dict1, "/sub/*" );

      TS_ASSERT_EQUALS( dict2.get<string>( "sub:word" ), new_dict1.get<string>( "word" ) );
      TS_ASSERT_EQUALS( dict2.get<int>( "sub:integer" ), new_dict1.get<int>( "integer" ) );
      TS_ASSERT_EQUALS( dict2.get<float>( "sub:real" ), new_dict1.get<float>( "real" ) );

      remove( filename.c_str() );
   }

   void test_list()
   {
      options::dictionary dict;
      dict.add_option( new options::string( "first" ) );
      dict.add_option( new options::list<options::integer>( "list_of_ints" ) );

      dict.compile();

      dict["first"] = "hello";
      dict["list_of_ints"] = "5";
      dict["list_of_ints"] = "8";
      dict["list_of_ints"] = "9";

      options::xml xml;
      string filename = tmpnam( NULL );
      xml.write( filename, dict );

      options::dictionary new_dict;
      new_dict.add_option( new options::string( "first" ) );
      new_dict.add_option( new options::list<options::integer>( "list_of_ints" ) );
      new_dict.compile();

      xml.read( filename, new_dict );

      TS_ASSERT_EQUALS( dict.get<string>( "first" ), new_dict.get<string>( "first" ) );
      TS_ASSERT_EQUALS( dict.get_list<int>( "list_of_ints" ), new_dict.get_list<int>( "list_of_ints" ) );

      remove( filename.c_str() );
   }
};
