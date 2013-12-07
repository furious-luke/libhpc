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
#include "libhpc/logging/stdout.hh"

using namespace hpc;

class tags_suite : public CxxTest::TestSuite {
public:

   void test_ctor()
   {
#ifndef NLOG
      logging::stdout log;
      TS_ASSERT( log._cur_tags.empty() );
      TS_ASSERT( log._tags.empty() );
#endif
   }

   void test_push_tag()
   {
#ifndef NLOG
      logging::stdout log;
      log.push_tag( "hello" );
      TS_ASSERT_EQUALS( log._cur_tags.size(), 1 );
      TS_ASSERT_EQUALS( log._cur_tags.begin()->first, 0 );
      TS_ASSERT_EQUALS( log._cur_tags.begin()->second.size(), 1 );
      TS_ASSERT( log._cur_tags.begin()->second.begin()->first == "hello" );
      TS_ASSERT_EQUALS( log._cur_tags.begin()->second.begin()->second, 1 );
      TS_ASSERT( log._tags.empty() );
#endif
   }

   void test_pop_tag()
   {
#ifndef NLOG
      logging::stdout log;
      log.push_tag( "hello" );
      log.pop_tag( "hello" );
      TS_ASSERT_EQUALS( log._cur_tags.size(), 1 );
      TS_ASSERT_EQUALS( log._cur_tags.begin()->first, 0 );
      TS_ASSERT_EQUALS( log._cur_tags.begin()->second.size(), 0 );
      TS_ASSERT( log._tags.empty() );
#endif
   }

   void test_add_tag()
   {
#ifndef NLOG
      logging::stdout log;
      log.add_tag( "hello" );
      log.add_tag( "world" );
      log.add_tag( "hello" );
      TS_ASSERT_EQUALS( log._tags.size(), 2 );
      TS_ASSERT( log._tags.find( "hello" ) != log._tags.end() );
      TS_ASSERT( log._tags.find( "world" ) != log._tags.end() );
#endif
   }

   void test_visible_no_tags()
   {
#ifndef NLOG
      logging::stdout log;
      TS_ASSERT( log.visible() );
      log.push_tag( "hello" );
      TS_ASSERT( log.visible() );
      log.pop_tag( "hello" );
      TS_ASSERT( log.visible() );
#endif
   }

   void test_visible_set_tags()
   {
#ifndef NLOG
      logging::stdout log;
      TS_ASSERT( log.visible() );
      log.add_tag( "hello" );
      TS_ASSERT( !log.visible() );
      log.push_tag( "world" );
      TS_ASSERT( !log.visible() );
      log.push_tag( "hello" );
      TS_ASSERT( log.visible() );
#endif
   }
};
