#include <boost/iterator/zip_iterator.hpp>
#include <cxxtest/TestSuite.h>
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/sort_permute2_iter.hh"
#include "libhpc/containers/global.hh"

using namespace hpc;

class sort_perumte2_iter_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      vector<int> first(10);
      vector<float> second(10);
      vector<long> third(10);

      for(hpc::index ii = 0; ii < 10; ++ii) {
	 first[ii] = (int)(9 - ii);
	 second[ii] = (float)ii;
	 third[ii] = (long)ii;
      }

      std::sort(
	 make_sort_permute2_iter(first.begin(), second.begin(), third.begin()),
	 make_sort_permute2_iter(first.end(), second.end(), third.end()),
	 sort_permute2_iter_compare<vector<int>::iterator, vector<float>::iterator, vector<long>::iterator>()
	 );

      for(hpc::index ii = 0; ii < 10; ++ii) {
      	 TS_ASSERT_EQUALS(first[ii], ii);
      	 TS_ASSERT_EQUALS(second[ii], (float)(9 - ii));
      	 TS_ASSERT_EQUALS(third[ii], (long)(9 - ii));
      }
   }
};
