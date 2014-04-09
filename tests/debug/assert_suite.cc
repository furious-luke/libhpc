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

#include <libhpc/unit_test/main.hh>
#include <libhpc/debug/assert.hh>

TEST_CASE( "/libhpc/debug/assert/basic" )
{
   bool inside = false;
   try
   {
      ASSERT( 0 );
   }
#ifndef NDEBUG
   catch( hpc::debug::assertion& asrt )
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

TEST_CASE( "/libhpc/debug/assert/message" )
{
   bool inside = false;
   try
   {
      ASSERT( 0, "Integer: ", 100, "Real: ", 33.3 );
   }
#ifndef NDEBUG
   catch( hpc::debug::assertion& asrt )
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
