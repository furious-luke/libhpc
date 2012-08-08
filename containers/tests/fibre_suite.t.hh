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
#include "libhpc/containers/fibre.hh"

using namespace hpc;

class fibre_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      fibre<int> fbr;
      this->check_empty(fbr);
   }

   void test_fibre_size()
   {
      fibre<int> fbr;
      fbr.set_fibre_size(3);
      TS_ASSERT_EQUALS(fbr.fibre_size(), 3);
      fbr.set_fibre_size(2);
      TS_ASSERT_EQUALS(fbr.fibre_size(), 2);

      // Check that the fibre object is deallocated on chaging the fibre size.
      fbr.resize(10);
      fbr.set_fibre_size(1);
      TS_ASSERT_EQUALS(fbr.fibre_size(), 1);
      TS_ASSERT_EQUALS(fbr.size(), 0);
      TS_ASSERT_EQUALS(fbr.capacity(), 0);
   }

   void test_size() {
      fibre<int> fbr;
      fbr.set_fibre_size(3);
      fbr.resize(10);
      TS_ASSERT_EQUALS(fbr.size(), 10);
      TS_ASSERT_EQUALS(((hpc::vector<int>&)fbr).size(), 30);
   }

   void test_max_size() {
      fibre<int> fbr;

#ifndef NDEBUG
      // Should assert when attempted without a fibre size.
      TS_ASSERT_THROWS_ANYTHING(fbr.max_size());
#endif

      fbr.set_fibre_size(3);
      fibre<int>::size_type max_size = fbr.max_size();
      TS_ASSERT_EQUALS(((hpc::vector<int>&)fbr).max_size()/3, max_size);
   }

   void test_capacity() {
      fibre<int> fbr;
      fbr.set_fibre_size(3);
      fbr.resize(10);
      TS_ASSERT(fbr.capacity() >= 10);
      TS_ASSERT_EQUALS(((hpc::vector<int>&)fbr).capacity(), fbr.capacity()*3);
   }

   void test_reserve() {
      fibre<int> fbr;
      fbr.set_fibre_size(3);
      fbr.reserve(10);
      TS_ASSERT_EQUALS(fbr.size(), 0);
      TS_ASSERT(fbr.capacity() >= 10);
      TS_ASSERT_EQUALS(((hpc::vector<int>&)fbr).capacity(), fbr.capacity()*3);
   }

   void test_resize() {
      fibre<int> fbr;
      fbr.set_fibre_size(3);
      fbr.resize(10);
      TS_ASSERT_EQUALS(fbr.size(), 10);
      TS_ASSERT_EQUALS(((hpc::vector<int>&)fbr).size(), 30);
      TS_ASSERT(fbr.capacity() >= 10);
      TS_ASSERT_EQUALS(((hpc::vector<int>&)fbr).capacity(), fbr.capacity()*3);
   }

   void test_clear() {
      fibre<int> fbr;
      fbr.set_fibre_size(3);
      fbr.resize(10);
      fbr.clear();
      TS_ASSERT_EQUALS(fbr.size(), 0);
      TS_ASSERT(fbr.capacity() >= 10);
   }

   void test_deallocate() {
      fibre<int> fbr;
      fbr.set_fibre_size(3);
      fbr.resize(10);
      fbr.deallocate();
      TS_ASSERT_EQUALS(fbr.size(), 0);
      TS_ASSERT_EQUALS(fbr.capacity(), 0);
   }

   void test_const_dereference() {
      fibre<int> fbr;
      fbr.set_fibre_size(3);
      fbr.resize(10);
#ifndef NDEBUG
      TS_ASSERT_THROWS_ANYTHING(((const fibre<int>&)fbr)[-1]);
      TS_ASSERT_THROWS_ANYTHING(((const fibre<int>&)fbr)[11]);
#endif
      TS_ASSERT_THROWS_NOTHING(((const fibre<int>&)fbr)[3]);
   }

   void test_dereference() {
      fibre<int> fbr;
      fbr.set_fibre_size(3);
      fbr.resize(10);

      // TODO: Clear out the initial values to zeros.

#ifndef NDEBUG
      TS_ASSERT_THROWS_ANYTHING(((fibre<int>&)fbr)[-1]);
      TS_ASSERT_THROWS_ANYTHING(((fibre<int>&)fbr)[11]);
#endif
      TS_ASSERT_THROWS_NOTHING(((fibre<int>&)fbr)[3]);

      vector<int>::view view = fbr[3];
      view[0] = 10;
      view[1] = 20;
      view[2] = 30;
      TS_ASSERT_EQUALS(((vector<int>&)fbr)[9], 10);
      TS_ASSERT_EQUALS(((vector<int>&)fbr)[10], 20);
      TS_ASSERT_EQUALS(((vector<int>&)fbr)[11], 30);
   }

   void test_iterator()
   {
      fibre<int> fbr;
      fbr.set_fibre_size( 3 );
      fbr.resize( 10 );

      int count = 0;
      for( fibre<int>::iterator it = fbr.begin(); it != fbr.end(); ++it ) {
         TS_ASSERT_EQUALS( (*it).size(), 3 );
         (*it)[0] = count;
         (*it)[1] = 10*count;
         (*it)[2] = 20*count;
         ++count;
      }
      TS_ASSERT_EQUALS( count, 10 );
      count = 0;
      for( fibre<int>::iterator it = fbr.begin(); it != fbr.end(); ++it ) {
         TS_ASSERT_EQUALS( (*it)[0], count );
         TS_ASSERT_EQUALS( (*it)[1], 10*count );
         TS_ASSERT_EQUALS( (*it)[2], 20*count );
         ++count;
      }
   }

   void test_const_iterator()
   {
      fibre<int> fbr;
      fbr.set_fibre_size( 3 );
      fbr.resize( 10 );

      int count = 0;
      for( fibre<int>::const_iterator it = fbr.begin(); it != fbr.end(); ++it ) {
         TS_ASSERT_EQUALS( (*it).size(), 3 );
         ++count;
      }
      TS_ASSERT_EQUALS( count, 10 );
   }

   void check_empty(fibre<int>& fbr) {
      TS_ASSERT(fbr.empty());
   }
};
