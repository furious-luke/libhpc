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

#include <libhpc/unit_test/main_mpi.hh>
#include <libhpc/mpi/managed.hh>

struct manager
{
   manager()
      : seen( false )
   {
   }

   void
   process()
   {
      seen = true;
   }

   bool seen;
};

struct worker
{
   worker()
      : seen( false )
   {
   }

   void
   process()
   {
      seen = true;
   }

   bool seen;
};

TEST_CASE( "/hpc/mpi/managed/constructor" )
{
   hpc::mpi::managed<manager,worker> mgd;
   TEST( mgd.manager_rank() == 0 );
   TEST( mgd.manager() == (void*)0 );
   TEST( mgd.worker() == (void*)0 );
}

TEST_CASE( "/hpc/mpi/managed/process" )
{
   hpc::mpi::managed<manager,worker> mgd;
   mgd.process();
   if( mgd.am_manager() )
   {
      TEST( mgd.manager() != (void*)0 );
      TEST( mgd.manager()->seen == true );
      TEST( mgd.worker() == (void*)0 );
   }
   else
   {
      TEST( mgd.worker() != (void*)0 );
      TEST( mgd.worker()->seen == true );
      TEST( mgd.manager() == (void*)0 );
   }
}
