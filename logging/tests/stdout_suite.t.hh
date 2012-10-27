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
#include "libhpc/logging/stdout.hh"

using namespace hpc;

class stdout_suite : public CxxTest::TestSuite {
public:

   void test_ctor()
   {
#ifndef NLOG
      logging::stdout log;
#endif
   }

   void test_basic()
   {
#ifndef NLOG
      logging::stdout log;
      // std::cout << "\n\n";
      // log << "Hello";
      // log << " world!";
      // log << logging::endl;
      // log << setindent( 2 );
      // log << "Hello indent!\n";
      // std::cout << "\n";
#endif
   }
};
