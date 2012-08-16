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
#include "libhpc/containers/containers.hh"
#include "libhpc/regexp/re.hh"

using namespace hpc;

class compile_suite : public CxxTest::TestSuite {
public:

   void test_range_map()
   {
      range_map<int,int> rm;
      rm.insert( range<int>( 0, 10 ), 5 );
      rm.insert( range<int>( 2, 6 ), 4 );
   }
};
