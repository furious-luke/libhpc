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

class std_vector_suite : public CxxTest::TestSuite {
public:

  void test_default_ctor()
  {
    vector<int> vec;
    this->check_empty(vec);
  }

  void test_deallocate()
  {
    TS_ASSERT_EQUALS(this->vec.size(), 10);
    TS_ASSERT_DIFFERS(this->vec.capacity(), 0);
    this->vec.deallocate();
    TS_ASSERT(this->vec.size() == 0);
    TS_ASSERT(this->vec.capacity() == 0);
  }

  void test_reallocate()
  {
    TS_ASSERT_EQUALS(this->vec.size(), 10);
    TS_ASSERT_DIFFERS(this->vec.capacity(), 0);
    this->vec.reallocate(5);
    TS_ASSERT(this->vec.size() == 5);
    TS_ASSERT(this->vec.capacity() >= 5);
  }

  // void test_duplicate()
  // {
  //   vector<int> cpy;
  //   cpy.duplicate(this->vec);
  //   this->check_contents(cpy);
  //   this->check_contents(this->vec);
  //   TS_ASSERT_DIFFERS(cpy.data(), this->vec.data());
  // }

  void test_take()
  {
    vector<int> cpy;
    cpy.take(this->vec);
    this->check_contents(cpy);
    this->check_empty(this->vec);
  }

  void test_const_dereference()
  {
    TS_ASSERT_THROWS_NOTHING(((const vector<int>&)this->vec)[0]);
#ifndef NDEBUG
    TS_ASSERT_THROWS_ANYTHING(((const vector<int>&)this->vec)[-1]);
    TS_ASSERT_THROWS_ANYTHING(((const vector<int>&)this->vec)[11]);
#else
    TS_ASSERT_THROWS_NOTHING(((const vector<int>&)this->vec)[11]);
#endif
  }

  void test_dereference()
  {
    TS_ASSERT_THROWS_NOTHING(((vector<int>&)this->vec)[0]);
#ifndef NDEBUG
    TS_ASSERT_THROWS_ANYTHING(((vector<int>&)this->vec)[-1]);
    TS_ASSERT_THROWS_ANYTHING(((vector<int>&)this->vec)[11]);
#else
    TS_ASSERT_THROWS_NOTHING(((vector<int>&)this->vec)[11]);
#endif
  }

  void check_empty(vector<int>& vec)
  {
    TS_ASSERT(vec.empty());
  }

  void check_contents(vector<int>& vec)
  {
    TS_ASSERT_EQUALS(vec.size(), 10);
    for(int ii = 0; ii < 10; ++ii)
      TS_ASSERT_EQUALS(vec[ii], ii);
  }

  void setUp()
  {
    this->vec.resize(10);
    hpc::iota(vec.begin(), vec.end(), 0);
  }

private:

  vector<int> vec;
};
