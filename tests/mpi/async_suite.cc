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
#include <libhpc/mpi/async.hh>

typedef hpc::mpi::comm comm;

struct event_handler
   : public hpc::mpi::async::event_handler
{
   event_handler( int tag,
                  bool done = false )
      : hpc::mpi::async::event_handler( tag ),
        count( 0 ),
        done( done )
   {
   }

   virtual
   bool
   event( MPI_Status const& stat )
   {
      count += comm::world.recv<int>( stat );
      return done;
   }

   int count;
   bool done;
};

TEST_CASE( "/hpc/mpi/async/constructor" )
{
   hpc::mpi::async async;
   TEST( async.event_handlers().empty() == true );
}

TEST_CASE( "/hpc/mpi/async/run/serial" )
{
   hpc::mpi::async async( comm::self );
   async.run();
   TEST( true == true );
}

TEST_CASE( "/hpc/mpi/async/run/parallel" )
{
   comm wkrs_comm = comm::world.create_excl( 0 );
   if( comm::world.size() > 1 )
   {
      if( comm::world.rank() == 0 )
      {
         hpc::mpi::async async;
         event_handler eh_0( 0 ), eh_1( 1, true );
         async.add_event_handler( &eh_0 );
         async.add_event_handler( &eh_1 );
         async.run();
         TEST( eh_0.count == 3*(comm::world.size() - 1) );
         TEST( eh_1.count == 5 );
      }
      else
      {
         hpc::mpi::comm::world.send<int>( 3, 0, 0 );
         wkrs_comm.barrier();
         if( comm::world.rank() == 1 )
            hpc::mpi::comm::world.send<int>( 5, 0, 1 );
      }
      comm::world.barrier();
   }
}
