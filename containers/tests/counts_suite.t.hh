#include <cxxtest/TestSuite.h>
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/counts.hh"

using namespace hpc;

class counts_suite : public CxxTest::TestSuite {
public:

  void test_counts_to_displs() {
    vector<int> cnts, displs;
    for(int ii = 0; ii < 10; ++ii)
      cnts.push_back(ii);

    displs.resize(11);
    counts_to_displs(cnts.begin(), cnts.end(), displs.begin());
    int val = 0;
    for(int ii = 0; ii < 11; ++ii) {
      TS_ASSERT_EQUALS(displs[ii], val);
      if(ii < 10)
	val += cnts[ii];
    }
  }

  void test_counts_to_displs_inplace() {
    vector<int> vec;
    vec.resize(11);
    for(int ii = 0; ii < 10; ++ii)
      vec[ii] = ii;

    counts_to_displs(vec.begin(), 10);
    int val = 0;
    for(int ii = 0; ii < 11; ++ii) {
      TS_ASSERT_EQUALS(vec[ii], val);
      if(ii < 10)
	val += ii;
    }
  }

  void test_displs_to_counts() {
    vector<int> cnts, displs;
    int val = 0;
    for(int ii = 0; ii < 11; ++ii) {
      displs.push_back(val);
      val += ii;
    }

    cnts.resize(10);
    displs_to_counts(displs.begin(), displs.end(), cnts.begin());
    for(int ii = 0; ii < 10; ++ii)
      TS_ASSERT_EQUALS(cnts[ii], ii);
  }

  void test_displs_to_counts_inplace() {
    vector<int> vec;
    vec.resize(11);
    vec[0] = 0;
    for(int ii = 0; ii < 10; ++ii)
      vec[ii + 1] = vec[ii] + ii;

    displs_to_counts(vec.begin(), 11);
    for(int ii = 0; ii < 10; ++ii)
      TS_ASSERT_EQUALS(vec[ii], ii);
  }
};
