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
