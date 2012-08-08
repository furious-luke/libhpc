#include <cxxtest/TestSuite.h>
#include "libhpc/containers/multimap.hh"

using namespace hpc;

class multimap_suite : public CxxTest::TestSuite {
public:

  void test_default_ctor() {
    multimap<int, int> multimap;
    this->check_empty(multimap);
  }

  void check_empty(multimap<int, int>& multimap) {
    TS_ASSERT(multimap.empty());
  }
};
