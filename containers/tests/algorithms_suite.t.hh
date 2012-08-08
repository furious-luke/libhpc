#include <cxxtest/TestSuite.h>
#include <boost/range/algorithm_ext.hpp>
#include "libhpc/containers/algorithms.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/map.hh"

using namespace hpc;

class algorithms_suite : public CxxTest::TestSuite {
public:

   void test_sequence_to_map()
   {
      vector<int> vec(10);
      boost::iota(vec, 100);
      hpc::map<int, int> res;
      sequence_to_map(vec.begin(), vec.end(), res);
      for(int ii = 0; ii < 10; ++ii) {
	 TS_ASSERT_EQUALS(res.get(ii), 100 + ii);
      }
   }
};
