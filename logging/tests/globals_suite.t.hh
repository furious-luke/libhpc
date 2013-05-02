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
#include "libhpc/logging/file.hh"

using namespace hpc;

class globals_suite : public CxxTest::TestSuite {
public:

   void test_push_pop()
   {
#ifndef NLOG
      // logging::file* log = new logging::file( "test.log" );
      // logging::push( log );
      // logging::pop();
#endif
   }

   // void test_clear()
   // {
   //    logging::file log( "test.log" );
   //    logging::push( log );
   //    logging::clear();
   // }

   void test_log()
   {
#ifndef NLOG
      // LOG_PUSH( new logging::file( "test1.log" ) );
      // LOG_PUSH( new logging::file( "test2.log" ) );
      // LOG( "Testing.", logging::endl );
      // LOG( "This is on" );
      // LOG( " one line." );
      // LOG( logging::endl );
      // LOG( setindent( 2 ), indent, "Indented doubly.", logging::endl, setindent( -2 ) );
      // LOG( indent, "Not indented.", logging::endl );
      // LOG( "Also not indented...", logging::endl, setindent( 2 ) );
      // LOG( "... but this one is.", logging::endl );
      // LOG( setindent( -2 ) );
      // LOG_POP();
      // LOG_POP();
#endif
   }
};
