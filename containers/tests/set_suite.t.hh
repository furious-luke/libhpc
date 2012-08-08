#include <cxxtest/TestSuite.h>
#include "libhpc/containers/set.hh"

using namespace hpc;

class set_suite : public CxxTest::TestSuite {
public:

  void test_default_ctor() {
    set<int> set;
    this->check_empty(set);
  }

  void check_empty(set<int>& set) {
    TS_ASSERT(set.empty());
  }
};
