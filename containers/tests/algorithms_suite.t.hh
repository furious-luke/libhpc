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
