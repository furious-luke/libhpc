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
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/functors.hh"

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
      hpc::iota(vec.begin(), vec.end(), 0);
   }

private:
   vector<int> vec;
};
