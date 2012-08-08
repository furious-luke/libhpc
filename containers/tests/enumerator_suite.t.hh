#include <cxxtest/TestSuite.h>
#include <boost/range/algorithm_ext.hpp>
#include "libhpc/containers/enumerator.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/map.hh"

using namespace hpc;

class enumerator_suite : public CxxTest::TestSuite {
public:

   void test_default_ctor()
   {
      enumerator<vector<int>::iterator, int> en;
   }

   void test_copy_ctor()
   {
      // TODO
   }

   void test_traverse()
   {
      vector<int> vec(10);
      boost::iota(vec, 100);

      int ii = 0;
      for(enumerator<vector<int>::iterator, int> en = vec.begin(); en != vec.end(); ++en, ++ii) {
      	 TS_ASSERT_EQUALS((*en).first, ii);
      	 TS_ASSERT_EQUALS((*en).second, 100 + ii);
      }
   }

   void test_inverse_traverse()
   {
      vector<int> vec(10);
      boost::iota(vec, 100);

      int ii = 0;
      for(renumerator<vector<int>::iterator, int> en = vec.begin(); en != vec.end(); ++en, ++ii) {
      	 TS_ASSERT_EQUALS((*en).first, 100 + ii);
      	 TS_ASSERT_EQUALS((*en).second, ii);
      }
   }

   void test_pipeline()
   {
      vector<int> vec(10);
      boost::iota(vec, 100);

      hpc::map<int, int> out;
      std::copy(
	 enumerator<vector<int>::iterator, int>(vec.begin()),
	 enumerator<vector<int>::iterator, int>(vec.end()),
	 std::inserter(out, out.begin())
	 );
   }
};
