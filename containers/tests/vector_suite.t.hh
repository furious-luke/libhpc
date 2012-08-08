#include <numeric>
#include <cxxtest/TestSuite.h>
#include "libhpc/containers/vector.hh"

using namespace hpc;

class vector_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      vector<int> vec;
      this->check_empty(vec);
   }

   void test_view_conversion()
   {
      vector<int>::view view = this->vec;
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
      std::iota(vec.begin(), vec.end(), 0);
   }

private:
   vector<int> vec;
};
