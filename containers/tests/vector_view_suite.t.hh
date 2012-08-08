#include <numeric>
#include <cxxtest/TestSuite.h>
#include "libhpc/containers/std_vector.hh"

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
    std::iota(diff.begin(), diff.end(), 3);
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
    std::iota(vec.begin(), vec.end(), 0);
  }

private:
  vector<int> vec;
};
