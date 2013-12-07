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
#include "libhpc/algorithm/farmer.hh"
#include "libhpc/logging/logging.hh"
#include "libhpc/hpcmpi/logger.hh"

using namespace hpc;

struct master_type
{
   master_type()
      : base( 0 )
   {
   }

   void
   next( unsigned long long& first,
         unsigned long long& last )
   {
      first = base;
      if( base < 1000 )
         base += 10;
      last = base;
      LOGDLN( "Master base: ", base );
   }

   unsigned long base;
};

class farmer_suite : public CxxTest::TestSuite {
public:

   void test_all()
   {
      // LOG_PUSH( new mpi::logger( "log." ) );

      master_type master;
      algorithm::farmer farm( 232 );
      farm.initialise( master );

      if( mpi::comm::world.rank() == 0 )
      {
         while( farm.master( master ) )
         {
            // Any master processing?
         }
      }
      else
      {
         range<unsigned long long> rng;
         do
         {
            rng = farm.request();
         }
         while( rng.length() );
      }
   }
};
