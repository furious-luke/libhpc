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
#include "libhpc/containers/ring_buffer.hh"

using namespace hpc;

class ring_buffer_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      ring_buffer<int> rb;
   }

   void test_resize()
   {
      ring_buffer<int> rb;
      rb.resize( 10 );
      TS_ASSERT_EQUALS( rb._buf.size(), 10 );
      TS_ASSERT_EQUALS( rb._start, rb._buf.begin() );
      TS_ASSERT_EQUALS( rb._finish, rb._buf.begin() );
   }

   void test_insert_nothing()
   {
      ring_buffer<int> rb;
      rb.resize( 10 );
      ring_buffer<int>::size_type res;
      res = rb.insert( &res, &res );
      TS_ASSERT_EQUALS( res, 0 );
      TS_ASSERT_EQUALS( rb._start, rb._buf.begin() );
      TS_ASSERT_EQUALS( rb._finish, rb._start );
      TS_ASSERT_EQUALS( rb._size, 0 );
   }

   void test_insert_when_full()
   {
      ring_buffer<int> rb;
      rb.resize( 10 );
      ring_buffer<int>::size_type res;
      for( unsigned ii = 0; ii < 10; ++ii )
         rb.insert( &ii, &ii + 1 );
      res = rb.insert( &res, &res + 1 );
      TS_ASSERT_EQUALS( res, 0 );
      TS_ASSERT_EQUALS( rb._start, rb._buf.begin() );
      TS_ASSERT_EQUALS( rb._finish, rb._start + 10 );
      TS_ASSERT_EQUALS( rb._size, 10 );
   }

   void test_insert_one_element()
   {
      ring_buffer<int> rb;
      rb.resize( 10 );
      ring_buffer<int>::size_type res;
      for( unsigned ii = 0; ii < 10; ++ii )
      {
         res = rb.insert( &ii, &ii + 1 );
         TS_ASSERT_EQUALS( res, 1 );
         TS_ASSERT_EQUALS( rb._start, rb._buf.begin() );
         TS_ASSERT_EQUALS( rb._finish, rb._start + ii + 1 );
         TS_ASSERT_EQUALS( rb._size, ii + 1 );
      }
   }

   void test_insert_many()
   {
      ring_buffer<int> rb;
      rb.resize( 10 );
      vector<int> buf( 10 );
      hpc::iota( buf.begin(), buf.end(), 0 );
      ring_buffer<int>::size_type res;
      res = rb.insert( buf.begin(), buf.end() );
      TS_ASSERT_EQUALS( res, 10 );
      TS_ASSERT_EQUALS( rb._start, rb._buf.begin() );
      TS_ASSERT_EQUALS( rb._finish, rb._start + 10 );
      TS_ASSERT_EQUALS( rb._size, 10 );
   }

   void test_insert_many_full()
   {
      ring_buffer<int> rb;
      rb.resize( 10 );
      vector<int> buf( 8 );
      hpc::iota( buf.begin(), buf.end(), 0 );
      ring_buffer<int>::size_type res;
      res = rb.insert( buf.begin(), buf.end() );
      TS_ASSERT_EQUALS( res, 8 );
      TS_ASSERT_EQUALS( rb._start, rb._buf.begin() );
      TS_ASSERT_EQUALS( rb._finish, rb._start + 8 );
      TS_ASSERT_EQUALS( rb._size, 8 );
      res = rb.insert( buf.begin(), buf.end() );
      TS_ASSERT_EQUALS( res, 2 );
      TS_ASSERT_EQUALS( rb._start, rb._buf.begin() );
      // TS_ASSERT_EQUALS( rb._finish, rb._start + 10 );
      // TS_ASSERT_EQUALS( rb._size, 10 );
   }
};
