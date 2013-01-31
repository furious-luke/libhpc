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
#include "libhpc/logging/globals.hh"

using namespace hpc;

class instrument_suite : public CxxTest::TestSuite {
public:

   void test_all()
   {
#ifndef NLOG
      // LOG_CONSOLE();
      // func_a();
      // LOG_POP();
#endif
   }

   void func_a()
   {
      func_b();
      func_c();
   }

   void func_b()
   {
   }

   void func_c()
   {
      func_d();
   }

   void func_d()
   {
   }
};
