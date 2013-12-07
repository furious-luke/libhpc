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

#include <libhpc/debug/unit_test_main.hh>
#include <libhpc/logging/block.hh>

using namespace hpc;
using namespace hpc::test;

namespace {

   // TODO: Swap to a file so there is no output to console.
   test_case<> ANON(
      "/libhpc/logging/block/debug",
      "",
      []()
      {
         int num = 10;
         {
            LOGBLOCKD( "Some block with number: ", num );
            LOGDLN( "Blah." );
         }
         {
            LOGBLOCKD( "Some other block." );
            LOGDLN( "Another blah." );
         }
      }
      );

}
