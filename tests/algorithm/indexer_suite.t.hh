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
#include "libhpc/algorithm/indexer.hh"
#include "libhpc/logging/logging.hh"
#include "libhpc/hpcmpi/logger.hh"

using namespace hpc;

class indexer_suite : public CxxTest::TestSuite {
public:

   void test_all()
   {
      algorithm::indexer idxr( 0 );

      if( mpi::comm::world.rank() == 0 )
      {
         while( idxr._base != (mpi::comm::world.size() - 1)*10*10 )
         {
            idxr.master();
         }

         // Must have generated enough.
         TS_ASSERT_EQUALS( idxr._base, (mpi::comm::world.size() - 1)*10*10 );

         // TODO: No overlap.
      }
      else
      {
         // Call 10 times.
         vector<unsigned long long> bases( 10 );
         for( unsigned ii = 0; ii < 10; ++ii )
         {
            bases[ii] = idxr.request( 10 );
         }
      }
   }
};
