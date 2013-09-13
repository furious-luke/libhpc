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

#include <libhpc/mpi/unit_test_main.hh>
#include "libhpc/algorithm/farmer2.hh"

using namespace hpc;
using namespace hpc::test;

namespace {

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

      void
      idle()
      {
         LOGDLN( "Idle." );
      }

      unsigned long base;
   };

   struct worker_type
   {
      void
      process( unsigned long long first,
               unsigned long long last )
      {
         LOGDLN( first, " - ", last );
      }
   };

   test_case<> ANON(
      "/algorithm/farmer2",
      "",
      []()
      {
         algorithm::farmer2<master_type,worker_type> farm;
         farm.run();
      }
      );

}
