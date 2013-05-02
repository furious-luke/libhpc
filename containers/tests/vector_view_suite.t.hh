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
#include "libhpc/containers/std_vector.hh"
#include "libhpc/containers/functors.hh"

using namespace hpc::impl::std;

class vector_view_suite : public CxxTest::TestSuite {
public:

  void test_one_to_one_ctor()
  {
    vector<int>::view view(vec);
    this->check_contents(view);
  }

  void test_resize_ctor()
  {
    vector<int>::view view(vec, 5);
    TS_ASSERT_EQUALS(view.size(), 5);
    for(int ii = 0; ii < 5; ++ii)
      TS_ASSERT(view[ii] == ii);
  }

  void test_offset_ctor()
  {
    vector<int>::view view(vec, 7, 3);
    TS_ASSERT_EQUALS(view.size(), 7);
    for(int ii = 0; ii < 7; ++ii)
      TS_ASSERT(view[ii] == ii + 3);
  }

  void test_copy_ctor()
  {
    vector<int>::view view(vec);
    vector<int>::view copy(view);
    this->check_contents(view);
    this->check_contents(copy);
  }

  void test_size()
  {
    vector<int>::view view(this->vec);
    TS_ASSERT_EQUALS(view.size(), 10);
  }

  void test_empty()
  {
    {
      vector<int>::view view;
      TS_ASSERT(view.empty());
    }
    {
      vector<int>::view view(this->vec);
      TS_ASSERT(!view.empty());
    }
  }

  void test_data()
  {
    vector<int>::view view(this->vec);
    TS_ASSERT_EQUALS(view.data(), this->vec.data());
  }

  void test_const_dereference()
  {
    vector<int>::view view(this->vec);
    TS_ASSERT_THROWS_NOTHING(((const vector<int>::view&)view)[0]);
#ifndef NDEBUG
    TS_ASSERT_THROWS_ANYTHING(((const vector<int>::view&)view)[-1]);
    TS_ASSERT_THROWS_ANYTHING(((const vector<int>::view&)view)[11]);
#else
    TS_ASSERT_THROWS_NOTHING(((const vector<int>::view&)view)[11]);
#endif
  }

  void test_dereference()
  {
    vector<int>::view view(this->vec);
    TS_ASSERT_THROWS_NOTHING(((vector<int>::view&)view)[0]);
#ifndef NDEBUG
    TS_ASSERT_THROWS_ANYTHING(((vector<int>::view&)view)[-1]);
    TS_ASSERT_THROWS_ANYTHING(((vector<int>::view&)view)[11]);
#else
    TS_ASSERT_THROWS_NOTHING(((vector<int>::view&)view)[11]);
#endif
  }

   void test_assignment()
   {
      vector<int> tmp(10);
      vector<int>::view view(tmp);
      vector<int>::view src(this->vec);
      view = src;
      for(int ii = 0; ii < view.size(); ++ii)
	 TS_ASSERT_EQUALS(view[ii], src[ii]);
   }

  void test_const_begin()
  {
    const vector<int>::view view(this->vec);
    TS_ASSERT_EQUALS(*view.begin(), 0);
    TS_ASSERT_EQUALS(*++view.begin(), 1);
  }

  void test_begin()
  {
    vector<int>::view view(this->vec);
    TS_ASSERT_EQUALS(*view.begin(), 0);
    TS_ASSERT_EQUALS(*++view.begin(), 1);
  }

  void test_const_end()
  {
    const vector<int>::view view(this->vec);
    TS_ASSERT_EQUALS(*--view.end(), 9);
    TS_ASSERT_EQUALS(*----view.end(), 8);
  }

  void test_end()
  {
    vector<int>::view view(this->vec);
    TS_ASSERT_EQUALS(*--view.end(), 9);
    TS_ASSERT_EQUALS(*----view.end(), 8);
  }

  void test_equality()
  {
    vector<int>::view view(this->vec);
    vector<int>::view copy(view);
    vector<int>::view offs(this->vec, 7, 3);
    vector<int> diff(10);
    hpc::iota(diff.begin(), diff.end(), 3);
    TS_ASSERT(view == copy);
    TS_ASSERT(!(view == offs));
    TS_ASSERT(!(view == diff));
  }

  void check_contents(vector<int>::view& view)
  {
    TS_ASSERT_EQUALS(view.size(), 10);
    for(int ii = 0; ii < 10; ++ii)
      TS_ASSERT(view[ii] == ii);
  }

  void setUp()
  {
    this->vec.resize(10);
    hpc::iota(vec.begin(), vec.end(), 0);
  }

private:

  vector<int> vec;
};
