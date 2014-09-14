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
#include <libhpc/mpi/helpers.hh>

typedef hpc::mpi::comm comm;

SUITE_PREFIX( "/hpc/mpi/helpers/" );

TEST_CASE( "balanced_local_size" )
{
   TEST( hpc::mpi::balanced_local_size( 0 ) == 0 );
   if( comm::world.size() == 1 )
   {
      TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
      TEST( hpc::mpi::balanced_local_size( 2 ) == 2 );
   }
   else if( comm::world.size() == 2 )
   {
      if( comm::world.rank() == 0 )
      {
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 0 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 2 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 2 ) == 2 );
	 TEST( hpc::mpi::balanced_local_size( 2 ) == 2 );
      }
      else
      {
	 TEST( hpc::mpi::balanced_local_size( 0 ) == 0 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 0 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 0 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 2 ) == 2 );
      }
   }
   else if( comm::world.size() == 3 )
   {
      if( comm::world.rank() == 0 )
      {
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 2 ) == 2 );
	 TEST( hpc::mpi::balanced_local_size( 2 ) == 2 );
	 TEST( hpc::mpi::balanced_local_size( 2 ) == 2 );
      }
      else if( comm::world.rank() == 1 )
      {
	 TEST( hpc::mpi::balanced_local_size( 0 ) == 0 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 2 ) == 2 );
	 TEST( hpc::mpi::balanced_local_size( 2 ) == 2 );
      }
      else
      {
	 TEST( hpc::mpi::balanced_local_size( 0 ) == 0 );
	 TEST( hpc::mpi::balanced_local_size( 0 ) == 0 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_local_size( 2 ) == 2 );
      }
   }
}

TEST_CASE( "balanced_left_size" )
{
   TEST( hpc::mpi::balanced_left_size( 0 ) == 0 );
   if( comm::world.size() == 1 )
   {
      TEST( hpc::mpi::balanced_left_size( 1 ) == 1 );
      TEST( hpc::mpi::balanced_left_size( 2 ) == 1 );
      TEST( hpc::mpi::balanced_left_size( 3 ) == 2 );
      TEST( hpc::mpi::balanced_left_size( 4 ) == 2 );
      TEST( hpc::mpi::balanced_left_size( 5 ) == 3 );
   }
   else if( comm::world.size() == 2 )
   {
      if( comm::world.rank() == 0 )
      {
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_left_size( 2 ) == 2 );
	 TEST( hpc::mpi::balanced_left_size( 2 ) == 2 );
	 TEST( hpc::mpi::balanced_left_size( 3 ) == 3 );
	 TEST( hpc::mpi::balanced_left_size( 3 ) == 3 );
      }
      else
      {
	 TEST( hpc::mpi::balanced_left_size( 0 ) == 1 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 2 );
	 TEST( hpc::mpi::balanced_left_size( 2 ) == 2 );
	 TEST( hpc::mpi::balanced_left_size( 2 ) == 3 );
	 TEST( hpc::mpi::balanced_left_size( 3 ) == 3 );
      }
   }
   else if( comm::world.size() == 3 )
   {
      if( comm::world.rank() == 0 )
      {
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 1 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 2 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 2 );
	 TEST( hpc::mpi::balanced_left_size( 2 ) == 3 );
	 TEST( hpc::mpi::balanced_left_size( 2 ) == 4 );
	 TEST( hpc::mpi::balanced_left_size( 2 ) == 4 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 201 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 202 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 202 );
      }
      else if( comm::world.rank() == 1 )
      {
	 TEST( hpc::mpi::balanced_left_size( 0 ) == 1 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 2 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 2 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 3 );
	 TEST( hpc::mpi::balanced_left_size( 2 ) == 4 );
	 TEST( hpc::mpi::balanced_left_size( 2 ) == 4 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 201 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 202 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 202 );
      }
      else
      {
	 TEST( hpc::mpi::balanced_left_size( 0 ) == 1 );
	 TEST( hpc::mpi::balanced_left_size( 0 ) == 2 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 2 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 3 );
	 TEST( hpc::mpi::balanced_left_size( 1 ) == 4 );
	 TEST( hpc::mpi::balanced_left_size( 2 ) == 4 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 201 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 202 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 202 );
      }
   }
   else if( comm::world.size() == 5 )
   {
      if( comm::world.rank() == 0 )
      {
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 300 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 301 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 302 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
      }
      else if( comm::world.rank() == 1 )
      {
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 300 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 301 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 302 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
      }
      else if( comm::world.rank() == 2 )
      {
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 300 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 301 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 302 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
      }
      else if( comm::world.rank() == 3 )
      {
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 300 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 301 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 302 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 303 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
      }
      else
      {
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 300 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 301 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 302 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 303 );
	 TEST( hpc::mpi::balanced_left_size( 100 ) == 303 );
	 TEST( hpc::mpi::balanced_left_size( 101 ) == 303 );
      }
   }
}
