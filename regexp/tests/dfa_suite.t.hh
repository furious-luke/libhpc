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
#include "libhpc/logging/logging.hh"
#include "libhpc/regexp/dfa.hh"
#include "libhpc/regexp/syntax_tree.hh"

using namespace hpc;

class dfa_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      re::dfa dfa;
   }

   void test_conversion()
   {
      re::syntax_tree st;
      st.construct( "(one)|(two)|(three)" );
      re::dfa dfa;
      st.to_dfa( dfa );
   }

   void test_simple()
   {
      re::syntax_tree st;
      st.construct( "(a)|(b)|(c)" );
      re::dfa dfa;
      st.to_dfa( dfa );
   }

   void test_match()
   {
      re::dfa dfa( "(o(n)e)|(two)|(t(h(r)e)e)" );
      TS_ASSERT( dfa.match( "one" ) );
      TS_ASSERT( dfa.match( "two" ) );
      TS_ASSERT( dfa.match( "three" ) );
      TS_ASSERT( !dfa.match( "hello" ) );
      TS_ASSERT( !dfa.match( "oneo" ) );
      TS_ASSERT( !dfa.match( "onethree" ) );
   }

   void test_match_last_capture()
   {
      re::match match;
      re::dfa dfa( "(o(n)e)|(two)|(t(h(r)e)e)" );
      dfa.match( "one", match );
      TS_ASSERT( match.last_capture() == 0 );
      dfa.match( "two", match );
      TS_ASSERT( match.last_capture() == 1 );
      dfa.match( "three", match );
      TS_ASSERT( match.last_capture() == 2 );
   }

   void test_similar_split()
   {
      re::syntax_tree st;
      st.construct( "abc|ebc" );
      re::dfa dfa;
      st.to_dfa( dfa );
   }

   void test_converge()
   {
      re::syntax_tree st;
      st.construct( "(a|b)c" );
      re::dfa dfa;
      st.to_dfa( dfa );
   }

   void test_search()
   {
      re::match match;
      re::dfa dfa( "(one)|(two)|(three)" );
      TS_ASSERT( dfa.search( "one", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 0 );
      TS_ASSERT( dfa.search( "two", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 1 );
      TS_ASSERT( dfa.search( "three", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 2 );
      TS_ASSERT( !dfa.search( "hello", match ) );
      TS_ASSERT( !dfa.search( "0one", match ) );
      TS_ASSERT( dfa.search( "onethree", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 0 );
      TS_ASSERT( dfa.search( "threelade", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 2 );
   }

   void test_match_hidden()
   {
      re::dfa dfa( ".*(?:deg|ef)" );
      re::match match;
      TS_ASSERT( dfa.match( "abcdeg", match ) );
      TS_ASSERT( dfa.match( "abcef", match ) );
      TS_ASSERT( dfa.match( "abcdef", match ) );
   }

   void test_match_hidden_capture()
   {
      re::dfa dfa( ".*(?:(deg)|(ef))" );
      re::match match;
      TS_ASSERT( dfa.match( "abcdeg", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 0 );
      TS_ASSERT( dfa.match( "abcef", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 1 );
      TS_ASSERT( dfa.match( "abcdef", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 1 );
   }

   ///
   /// As we process the 'ddd' part of the RE, each 'd' could be considered
   /// either a part of the uncaptured '.*' section, or a part of the 'ddd'
   /// section. It's possible the system will restart the capture on each 'd',
   /// which we don't want.
   ///
   // void test_match_ambiguous_capture()
   // {
   //    LOG_FILE( "test.log" );
   //    re::dfa dfa( ".*(?:(ddd)|(ef))" );
   //    re::match match;
   //    const char* str0 = "abcddd";
   //    TS_ASSERT( dfa.match( str0, match ) );
   //    TS_ASSERT_EQUALS( match.last_capture(), 0 );
   //    TS_ASSERT_EQUALS( match.capture( 0 ).first, str0 + 3 );
   //    TS_ASSERT_EQUALS( match.capture( 0 ).second, str0 + 6 );
   //    TS_ASSERT( dfa.match( "abcdddd", match ) );
   //    TS_ASSERT_EQUALS( match.last_capture(), 0 );
   //    TS_ASSERT_EQUALS( match.capture( 0 ).first, str0 + 4 );
   //    TS_ASSERT_EQUALS( match.capture( 0 ).second, str0 + 7 );
   // }

   void test_many()
   {
      re::dfa dfa( "ab*" );
      re::match match;
      TS_ASSERT( dfa.match( "a", match ) );
      TS_ASSERT( dfa.match( "ab", match ) );
      TS_ASSERT( dfa.match( "abb", match ) );
      TS_ASSERT( dfa.match( "abbb", match ) );
      TS_ASSERT( !dfa.match( "abbba", match ) );
   }

   void test_many_grouped()
   {
      re::dfa dfa( "a(?:bc)*" );
      re::match match;
      TS_ASSERT( dfa.match( "a", match ) );
      TS_ASSERT( !dfa.match( "ab", match ) );
      TS_ASSERT( !dfa.match( "ac", match ) );
      TS_ASSERT( dfa.match( "abc", match ) );
      TS_ASSERT( !dfa.match( "abca", match ) );
      TS_ASSERT( !dfa.match( "abcb", match ) );
      TS_ASSERT( !dfa.match( "abcc", match ) );
      TS_ASSERT( dfa.match( "abcbc", match ) );
      TS_ASSERT( !dfa.match( "abcbca", match ) );
   }

   void test_many_first_group()
   {
      re::dfa dfa( "(?:bc)*" );
      re::match match;
      TS_ASSERT( !dfa.match( "a", match ) );
      TS_ASSERT( !dfa.match( "b", match ) );
      TS_ASSERT( !dfa.match( "c", match ) );
      TS_ASSERT( dfa.match( "bc", match ) );
      TS_ASSERT( !dfa.match( "bcb", match ) );
      TS_ASSERT( !dfa.match( "bcc", match ) );
      TS_ASSERT( dfa.match( "bcbc", match ) );
   }

   void test_many_first_group_trailing()
   {
      re::dfa dfa( "(?:bc)*de" );
      re::match match;
      TS_ASSERT( !dfa.match( "a", match ) );
      TS_ASSERT( !dfa.match( "b", match ) );
      TS_ASSERT( !dfa.match( "c", match ) );
      TS_ASSERT( !dfa.match( "bc", match ) );
      TS_ASSERT( !dfa.match( "bcb", match ) );
      TS_ASSERT( !dfa.match( "bcc", match ) );
      TS_ASSERT( !dfa.match( "bcbc", match ) );
      TS_ASSERT( !dfa.match( "bde", match ) );
      TS_ASSERT( dfa.match( "bcde", match ) );
      TS_ASSERT( dfa.match( "bcbcde", match ) );
   }

   void test_many_grouped_trailing()
   {
      re::dfa dfa( "a(?:bc)*de" );
      re::match match;
      TS_ASSERT( !dfa.match( "a", match ) );
      TS_ASSERT( !dfa.match( "b", match ) );
      TS_ASSERT( !dfa.match( "c", match ) );
      TS_ASSERT( !dfa.match( "bc", match ) );
      TS_ASSERT( !dfa.match( "abc", match ) );
      TS_ASSERT( !dfa.match( "abcb", match ) );
      TS_ASSERT( !dfa.match( "abcc", match ) );
      TS_ASSERT( !dfa.match( "abcbc", match ) );
      TS_ASSERT( !dfa.match( "abde", match ) );
      TS_ASSERT( dfa.match( "abcde", match ) );
      TS_ASSERT( dfa.match( "abcbcde", match ) );
   }

   void test_plus()
   {
      re::dfa dfa( "ab+" );
      re::match match;
      TS_ASSERT( !dfa.match( "a", match ) );
      TS_ASSERT( dfa.match( "ab", match ) );
      TS_ASSERT( dfa.match( "abb", match ) );
      TS_ASSERT( dfa.match( "abbb", match ) );
      TS_ASSERT( !dfa.match( "abbba", match ) );
   }

   void test_plus_grouped()
   {
      re::dfa dfa( "a(?:bc)+" );
      re::match match;
      TS_ASSERT( !dfa.match( "a", match ) );
      TS_ASSERT( !dfa.match( "ab", match ) );
      TS_ASSERT( !dfa.match( "ac", match ) );
      TS_ASSERT( dfa.match( "abc", match ) );
      TS_ASSERT( !dfa.match( "abca", match ) );
      TS_ASSERT( !dfa.match( "abcb", match ) );
      TS_ASSERT( !dfa.match( "abcc", match ) );
      TS_ASSERT( dfa.match( "abcbc", match ) );
      TS_ASSERT( !dfa.match( "abcbca", match ) );
   }

   void test_no_repeat_captures()
   {
      re::dfa dfa;
      TS_ASSERT_THROWS_ANYTHING( dfa.construct( "a(bc)*" ) );
      TS_ASSERT_THROWS_ANYTHING( dfa.construct( "a(bc)+" ) );
   }

   void test_class_all()
   {
      re::dfa dfa( "a.c" );
      re::match match;
      TS_ASSERT( !dfa.match( "a", match ) );
      TS_ASSERT( !dfa.match( "c", match ) );
      TS_ASSERT( !dfa.match( "ac", match ) );
      TS_ASSERT( dfa.match( "abc", match ) );
      TS_ASSERT( dfa.match( "azc", match ) );
      TS_ASSERT( dfa.match( "a c", match ) );
      TS_ASSERT( dfa.match( "a$c", match ) );
      TS_ASSERT( !dfa.match( "abcd", match ) );
   }

   void test_class_all_repeat()
   {
      re::dfa dfa( "a.*c" );
      re::match match;
      TS_ASSERT( !dfa.match( "a", match ) );
      TS_ASSERT( !dfa.match( "c", match ) );
      TS_ASSERT( !dfa.match( "ad", match ) );
      TS_ASSERT( dfa.match( "ac", match ) );
      TS_ASSERT( dfa.match( "abc", match ) );
      TS_ASSERT( dfa.match( "azc", match ) );
      TS_ASSERT( dfa.match( "a c", match ) );
      TS_ASSERT( dfa.match( "a$c", match ) );
      TS_ASSERT( !dfa.match( "abcd", match ) );
      TS_ASSERT( dfa.match( "abcc", match ) );
      TS_ASSERT( dfa.match( "abc skd77 c", match ) );
   }

   void test_class_digit()
   {
      re::dfa dfa( "a\\dc" );
      re::match match;
      TS_ASSERT( !dfa.match( "a", match ) );
      TS_ASSERT( !dfa.match( "c", match ) );
      TS_ASSERT( !dfa.match( "ac", match ) );
      TS_ASSERT( !dfa.match( "adc", match ) );
      TS_ASSERT( dfa.match( "a0c", match ) );
      TS_ASSERT( dfa.match( "a1c", match ) );
      TS_ASSERT( dfa.match( "a4c", match ) );
      TS_ASSERT( dfa.match( "a9c", match ) );
      TS_ASSERT( !dfa.match( "a3cd", match ) );
      TS_ASSERT( !dfa.match( "a2", match ) );
   }

   void test_class_neg_ws()
   {
      re::dfa dfa( "a\\Wc" );
      re::match match;
      TS_ASSERT( !dfa.match( "a", match ) );
      TS_ASSERT( !dfa.match( "c", match ) );
      TS_ASSERT( !dfa.match( "ac", match ) );
      TS_ASSERT( !dfa.match( "a c", match ) );
      TS_ASSERT( !dfa.match( "a\nc", match ) );
      TS_ASSERT( dfa.match( "aac", match ) );
      TS_ASSERT( dfa.match( "abc", match ) );
      TS_ASSERT( dfa.match( "a!c", match ) );
      TS_ASSERT( dfa.match( "a~c", match ) );
      TS_ASSERT( dfa.match( "a0c", match ) );
   }

   void test_class_digit_alternation()
   {
      SET_ABORT( true );
      re::dfa dfa( "a(\\d|bc)d" );
      re::match match;
      TS_ASSERT( !dfa.match( "a", match ) );
      TS_ASSERT( !dfa.match( "c", match ) );
      TS_ASSERT( !dfa.match( "b", match ) );
      TS_ASSERT( !dfa.match( "d", match ) );
      TS_ASSERT( dfa.match( "abcd", match ) );
      TS_ASSERT( dfa.match( "a8d", match ) );
      TS_ASSERT( !dfa.match( "a0bcd", match ) );
      TS_ASSERT( !dfa.match( "abc3d", match ) );
      TS_ASSERT( !dfa.match( "abcde", match ) );
      TS_ASSERT( !dfa.match( "a7de", match ) );
   }

   void test_match_continued()
   {
      re::dfa dfa( "hello world!" );
      re::match match;
      string buf = "hello world!";

      TS_ASSERT_EQUALS( dfa.match_start( buf.begin(), buf.end(), match ), 1 );

      TS_ASSERT_EQUALS( dfa.match_start( buf.begin(), buf.begin() + 4, match ), -1 );
      TS_ASSERT_EQUALS( dfa.match_continue( buf.end(), match ), 1 );

      TS_ASSERT_EQUALS( dfa.match_start( buf.begin(), buf.begin() + 4, match ), -1 );
      TS_ASSERT_EQUALS( dfa.match_continue( buf.begin() + 5, match ), -1 );
      TS_ASSERT_EQUALS( dfa.match_continue( buf.begin() + 6, match ), -1 );
      TS_ASSERT_EQUALS( dfa.match_continue( buf.end(), match ), 1 );

      buf = "hello worldy!";
      TS_ASSERT_EQUALS( dfa.match_start( buf.begin(), buf.begin() + 4, match ), -1 );
      TS_ASSERT_EQUALS( dfa.match_continue( buf.end(), match ), 0 );

      buf = "haello world!";
      TS_ASSERT_EQUALS( dfa.match_start( buf.begin(), buf.begin() + 4, match ), 0 );
   }

   void test_http()
   {
      re::dfa dfa( "HTTP(\\d)\\.(\\d)" );
      re::match match;
      string buf = "HTTP1.2";
      TS_ASSERT( dfa.match( buf, match ) );
      TS_ASSERT_EQUALS( buf[match.capture( 0 ).first], '1' );
      TS_ASSERT_EQUALS( buf[match.capture( 1 ).first], '2' );
      TS_ASSERT( !dfa.match( "HTTP14.2", match ) );
      TS_ASSERT( !dfa.match( "HTTP1.", match ) );
      TS_ASSERT( !dfa.match( "HTTP.3", match ) );
   }

   void test_http_request()
   {
      LOG_FILE( "test.log" );
      // string path = "";
      // string uri = "\\*|http://|/";
      re::dfa dfa( "(GET|POST|PUT) (\\W\\W*) HTTP\\d\\d*\\.\\d\\d*\n" );
      re::match match;
      string buf = "GET /index.html HTTP1.1\n";
      TS_ASSERT( dfa.match( buf, match ) );
      TS_ASSERT_EQUALS( match.capture( 0 ).first, 0 );
      TS_ASSERT_EQUALS( match.capture( 0 ).second, 3 );
      TS_ASSERT_EQUALS( match.capture( 1 ).first, 4 );
      TS_ASSERT_EQUALS( match.capture( 1 ).second, 15 );
   }

   // void test_search_basic()
   // {
   //    re::dfa dfa( "bc" );
   //    re::match match;
   //    TS_ASSERT( !dfa.search( "a", match ) );
   //    TS_ASSERT( !dfa.search( "b", match ) );
   //    TS_ASSERT( !dfa.search( "c", match ) );
   //    TS_ASSERT( dfa.search( "abcd", match ) );
   //    TS_ASSERT( dfa.search( "bcd", match ) );
   //    TS_ASSERT( dfa.search( "abc", match ) );
   // }

   // void test_search_hidden()
   // {
   //    re::dfa dfa( "(bcdf)|(cd)" );
   //    re::match match;

   //    // This should match, but which will it match?
   //    TS_ASSERT( dfa.search( "bcdf", match ) );
   //    TS_ASSERT_EQUALS( match.last_capture(), 0 );

   //    // This should match, because "cd" is in the second option.
   //    TS_ASSERT( dfa.search( "bcde", match ) );
   //    TS_ASSERT_EQUALS( match.last_capture(), 1 );
   // }
};
