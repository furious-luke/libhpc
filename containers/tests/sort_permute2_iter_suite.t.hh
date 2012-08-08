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
