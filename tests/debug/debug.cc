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

#include "libhpc/debug/unit_test.hh"
#include "libhpc/debug/assert.hh"
#include "libhpc/debug/except.hh"

using namespace hpc;
using namespace hpc::test;

///
///
///
test_case<> ANON(
   "/debug/assert/basic",
   "test basic usage",
   []()
   {
      bool inside = false;
      try
      {
         ASSERT( 0 );
      }
#ifndef NDEBUG
      catch( debug::assertion& asrt )
#else
      catch( std::exception& asrt )
#endif
      {
         inside = true;
      }
#ifndef NDEBUG
      TEST( inside == true, "Should have caught an assertion." );
#else
      TEST( inside == false, "Should not have caught an assertion." );
#endif
   }
   );

///
///
///
test_case<> ANON(
   "/debug/assert/message",
   "make sure messages are passed correctly",
   []()
   {
      bool inside = false;
      try
      {
         ASSERT( 0, "Integer: ", 100, "Real: ", 33.3 );
      }
#ifndef NDEBUG
      catch( debug::assertion& asrt )
#else
      catch( std::exception& asrt )
#endif
      {
         inside = true;
      }
#ifndef NDEBUG
      TEST( inside == true, "Should have caught an assertion." );
#else
      TEST( inside == false, "Should not have caught an assertion." );
#endif
   }
   );

///
///
///
test_case<> ANON(
   "/debug/except/basic",
   "",
   []()
   {
      bool inside = false;
      try
      {
         EXCEPT( 0 );
      }
      catch( std::exception& ex )
      {
         inside = true;
      }
      TEST( inside == true );
   }
   );
