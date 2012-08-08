#include <cxxtest/TestSuite.h>
#include "libhpc/containers/map.hh"

using namespace hpc;

class map_suite : public CxxTest::TestSuite {
public:

  void test_default_ctor() {
    map<int, int> map;
    this->check_empty(map);
  }

  void check_empty(map<int, int>& map) {
    TS_ASSERT(map.empty());
  }
};
