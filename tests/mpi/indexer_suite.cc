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
#include <libhpc/mpi/indexer.hh>
#include <libhpc/system/varray.hh>

typedef hpc::mpi::comm comm;

TEST_CASE( "/hpc/mpi/indexer/constructor" )
{
   hpc::mpi::indexer<int> idxr( 3 );
   TEST( idxr.tag() == 3 );
   // TEST( idxr.comm() == comm::world );
   TEST( idxr.base() == 0 );
}

TEST_CASE( "/hpc/mpi/indexer/serial/event" )
{
   hpc::mpi::indexer<int> idxr( 3, comm::self );
   MPI_Status stat;
   THROWS_ANY( idxr.event( stat ) );
}

TEST_CASE( "/hpc/mpi/indexer/serial/request" )
{
   hpc::mpi::indexer<int> idxr( 3, comm::self );
   TEST( idxr.request( 10 ) == 0 );
   TEST( idxr.request( 5 ) == 10 );
   TEST( idxr.request( 2 ) == 15 );
   TEST( idxr.base() == 17 );
}

TEST_CASE( "/hpc/mpi/indexer/parallel" )
{
   hpc::mpi::async async;
   hpc::mpi::indexer<int> idxr( 3 );
   async.add_event_handler( &idxr );
   async.set_max_events( 3*(comm::world.size() - 1) );
   std::vector<int> bases;
   if( !async.run() )
   {
      bases.push_back( idxr.request( 5 ) );
      bases.push_back( idxr.request( 5 ) );
      bases.push_back( idxr.request( 5 ) );
   }
   else
   {
      TEST( idxr.base() == 15*(comm::world.size() - 1) );
   }
   if( comm::world.size() > 1 )
   {
      std::vector<int> all_bases = comm::world.all_gatherv( bases );
      TEST( hpc::has_duplicates( all_bases ) == false );
      TEST( *std::max_element( all_bases.begin(), all_bases.end() ) == 15*(comm::world.size() - 1) - 5 );
      TEST( *std::min_element( all_bases.begin(), all_bases.end() ) == 0 );
   }
}

TEST_CASE( "/hpc/mpi/indexer/parallel/array" )
{
   hpc::mpi::async async;
   hpc::mpi::indexer<hpc::varray<int,2> > idxr( 3 );
   async.add_event_handler( &idxr );
   async.set_max_events( 3*(comm::world.size() - 1) );
   std::vector<hpc::varray<int,2> > bases;
   if( !async.run() )
   {
      bases.push_back( idxr.request( hpc::varray<int,2>{ 5, 2 } ) );
      bases.push_back( idxr.request( hpc::varray<int,2>{ 5, 2 } ) );
      bases.push_back( idxr.request( hpc::varray<int,2>{ 5, 2 } ) );
   }
   else
   {
      hpc::varray<int,2> val{ 15*(comm::world.size() - 1), 6*(comm::world.size() - 1) };
      TEST( idxr.base() == val );
   }
   if( comm::world.size() > 1 )
   {
      std::vector<hpc::varray<int,2> > all_bases = comm::world.all_gatherv( bases );
      TEST( hpc::has_duplicates( all_bases ) == false );
      hpc::varray<int,2> val{ 15*(comm::world.size() - 1) - 5, 6*(comm::world.size() - 1) - 2 };
      TEST( *std::max_element( all_bases.begin(), all_bases.end() ) == val );
      val[0] = 0; val[1] = 0;
      TEST( *std::min_element( all_bases.begin(), all_bases.end() ) == val );
   }
}
