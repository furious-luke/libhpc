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

   void test_match_start()
   {
      re::match match;
      re::dfa dfa( "(one)|(two)|(three)" );
      TS_ASSERT( dfa.match_start( "one", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 0 );
      TS_ASSERT( dfa.match_start( "two", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 1 );
      TS_ASSERT( dfa.match_start( "three", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 2 );
      TS_ASSERT( !dfa.match_start( "hello", match ) );
      TS_ASSERT( !dfa.match_start( "0one", match ) );
      TS_ASSERT( dfa.match_start( "onethree", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 0 );
      TS_ASSERT( dfa.match_start( "threelade", match ) );
      TS_ASSERT_EQUALS( match.last_capture(), 2 );
   }

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

   void test_no_repeat_captures()
   {
      re::dfa dfa;
      TS_ASSERT_THROWS_ANYTHING( dfa.construct( "a(bc)*" ) );
   }

   void test_class_all()
   {
      LOG_FILE( "test.log" );
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
      LOG_POP();
   }

   void test_class_all_repeat()
   {
      LOG_FILE( "test.log" );
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
      LOG_POP();
   }
};
