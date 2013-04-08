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
#include "libhpc/containers/po2_ring_buffer.hh"

using namespace hpc;

class po2_ring_buffer_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      po2_ring_buffer<int> rb;
   }

   void test_resize()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      TS_ASSERT_EQUALS( rb._buf.size(), 8 );
      TS_ASSERT_EQUALS( rb._start, 0 );
      TS_ASSERT_EQUALS( rb._size, 0 );
   }

   void test_insert_nothing()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      po2_ring_buffer<int>::size_type res;
      res = rb.insert( &res, &res );
      TS_ASSERT_EQUALS( res, 0 );
      TS_ASSERT_EQUALS( rb._start, 0 );
      TS_ASSERT_EQUALS( rb._size, 0 );
   }

   void test_insert_when_full()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      po2_ring_buffer<int>::size_type res;
      for( unsigned ii = 0; ii < 8; ++ii )
         rb.insert( &ii, &ii + 1 );
      res = rb.insert( &res, &res + 1 );
      TS_ASSERT_EQUALS( res, 0 );
      TS_ASSERT_EQUALS( rb._start, 0 );
      TS_ASSERT_EQUALS( rb._size, 8 );
   }

   void test_insert_one_element()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      po2_ring_buffer<int>::size_type res;
      for( unsigned ii = 0; ii < 8; ++ii )
      {
         res = rb.insert( &ii, &ii + 1 );
         TS_ASSERT_EQUALS( res, 1 );
         TS_ASSERT_EQUALS( rb._start, 0 );
         TS_ASSERT_EQUALS( rb._size, ii + 1 );
      }
   }

   void test_insert_many()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      vector<int> buf( 8 );
      hpc::iota( buf.begin(), buf.end(), 0 );
      po2_ring_buffer<int>::size_type res;
      res = rb.insert( buf.begin(), buf.end() );
      TS_ASSERT_EQUALS( res, 8 );
      TS_ASSERT_EQUALS( rb._start, 0 );
      TS_ASSERT_EQUALS( rb._size, 8 );
   }

   void test_insert_many_full()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      vector<int> buf( 6 );
      hpc::iota( buf.begin(), buf.end(), 0 );
      po2_ring_buffer<int>::size_type res;
      res = rb.insert( buf.begin(), buf.end() );
      TS_ASSERT_EQUALS( res, 6 );
      TS_ASSERT_EQUALS( rb._start, 0 );
      TS_ASSERT_EQUALS( rb._size, 6 );
      res = rb.insert( buf.begin(), buf.end() );
      TS_ASSERT_EQUALS( res, 2 );
      TS_ASSERT_EQUALS( rb._start, 0 );
      TS_ASSERT_EQUALS( rb._size, 8 );
   }

   void test_iterator()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      for( unsigned ii = 0; ii < 8; ++ii )
         rb.insert( ii );
      unsigned ii = 0;
      for( auto it = rb.begin(); it != rb.end(); ++it, ++ii )
         TS_ASSERT_EQUALS( *it, ii );
   }

   void test_first_vacant_chunk_full()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      for( unsigned ii = 0; ii < 8; ++ii )
         rb.insert( ii );
      auto chunk = rb.first_vacant_chunk();
      TS_ASSERT_EQUALS( chunk.size(), 0 );
   }

   void test_first_vacant_chunk_end()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      for( unsigned ii = 0; ii < 4; ++ii )
         rb.insert( ii );
      auto chunk = rb.first_vacant_chunk();
      TS_ASSERT_EQUALS( chunk.size(), 4 );
      TS_ASSERT_EQUALS( chunk.data(), rb._buf.data() + 4 );
   }

   void test_first_vacant_chunk_span()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      for( unsigned ii = 0; ii < 4; ++ii )
         rb.insert( ii );
      rb.consume( 4 );
      for( unsigned ii = 0; ii < 6; ++ii )
         rb.insert( ii );
      auto chunk = rb.first_vacant_chunk();
      TS_ASSERT_EQUALS( chunk.size(), 2 );
      TS_ASSERT_EQUALS( chunk.data(), rb._buf.data() + 2 );
   }

   void test_first_vacant_chunk_center()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      for( unsigned ii = 0; ii < 4; ++ii )
         rb.insert( ii );
      rb.consume( 4 );
      for( unsigned ii = 0; ii < 2; ++ii )
         rb.insert( ii );
      auto chunk = rb.first_vacant_chunk();
      TS_ASSERT_EQUALS( chunk.size(), 2 );
      TS_ASSERT_EQUALS( chunk.data(), rb._buf.data() + 6 );
   }

   void test_second_vacant_chunk_full()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      for( unsigned ii = 0; ii < 8; ++ii )
         rb.insert( ii );
      auto chunk = rb.second_vacant_chunk();
      TS_ASSERT_EQUALS( chunk.size(), 0 );
   }

   void test_second_vacant_chunk_end()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      for( unsigned ii = 0; ii < 4; ++ii )
         rb.insert( ii );
      auto chunk = rb.second_vacant_chunk();
      TS_ASSERT_EQUALS( chunk.size(), 0 );
   }

   void test_second_vacant_chunk_span()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      for( unsigned ii = 0; ii < 4; ++ii )
         rb.insert( ii );
      rb.consume( 4 );
      for( unsigned ii = 0; ii < 6; ++ii )
         rb.insert( ii );
      auto chunk = rb.second_vacant_chunk();
      TS_ASSERT_EQUALS( chunk.size(), 0 );
   }

   void test_second_vacant_chunk_center()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      for( unsigned ii = 0; ii < 4; ++ii )
         rb.insert( ii );
      rb.consume( 4 );
      for( unsigned ii = 0; ii < 2; ++ii )
         rb.insert( ii );
      auto chunk = rb.second_vacant_chunk();
      TS_ASSERT_EQUALS( chunk.size(), 4 );
      TS_ASSERT_EQUALS( chunk.data(), rb._buf.data() );
   }

   void test_extend()
   {
      po2_ring_buffer<int> rb;
      rb.resize( 8 );
      for( unsigned ii = 0; ii < 4; ++ii )
         rb.insert( ii );
      rb.consume( 2 );
      auto chunk = rb.first_vacant_chunk();
      chunk[0] = 10;
      chunk[1] = 20;
      rb.extend( 2 );
      TS_ASSERT_EQUALS( rb[0], 2 );
      TS_ASSERT_EQUALS( rb[1], 3 );
      TS_ASSERT_EQUALS( rb[2], 10 );
      TS_ASSERT_EQUALS( rb[3], 20 );
   }
};
