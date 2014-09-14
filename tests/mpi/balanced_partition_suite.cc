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

#include <boost/range/algorithm/fill.hpp>
#include <libhpc/unit_test/main_mpi.hh>
#include <libhpc/mpi/partition.hh>

typedef hpc::mpi::comm comm;
typedef hpc::mpi::balanced_partition bpart;

SUITE_PREFIX( "/hpc/mpi/balanced_partition/" );

TEST_CASE( "constructor" )
{
   bpart bp;
   TEST( bp.comm().mpi_comm() == comm::world.mpi_comm() );
}

TEST_CASE( "construct/all_left" )
{
   bpart bp;
   std::vector<int> part( comm::world.size()*4 );
   boost::fill( part, 0 );
   THROWS_ANY( bp.construct( part.begin(), part.end() ) );
}

TEST_CASE( "construct/all_right" )
{
   bpart bp;
   std::vector<int> part( comm::world.size()*4 );
   boost::fill( part, 1 );
   THROWS_ANY( bp.construct( part.begin(), part.end() ) );
}

TEST_CASE( "construct/slight_imbalance" )
{
   bpart bp;
   std::vector<int> part( comm::world.size()*4 );
   for( unsigned ii = 0; ii < part.size()/2 + 1; ++ii )
      part[ii] = 0;
   for( unsigned ii = part.size()/2 + 1; ii < part.size(); ++ii )
      part[ii] = 1;
   THROWS_ANY( bp.construct( part.begin(), part.end() ) );
}

TEST_CASE( "construct/twos" )
{
   bpart bp;
   std::vector<int> part( 2 );
   part[0] = 0;
   part[1] = 1;
   if( comm::world.size() > 1 && comm::world.size()%2 )
   {
      if( comm::world.rank() == 0 )
	 part[1] = 0;
   }
   bp.construct( part.begin(), part.end() );
   if( comm::world.size() == 1 )
   {
      TEST( bp.displs().size() == 0 );
      TEST( bp.indices().size() == 1 );
      TEST( bp.indices()[0] == 0 );
      TEST( bp.partners().size() == 0 );
   }
   else if( comm::world.size() == 2 )
   {
      TEST( bp.displs().size() == 2 );
      TEST( bp.displs()[0] == 0 );
      TEST( bp.displs()[1] == 1 );
      TEST( bp.indices().size() == 1 );
      TEST( bp.partners().size() == 1 );
      if( comm::world.rank() == 0 )
      {
	 TEST( bp.indices()[0] == 0 );
	 TEST( bp.partners()[0] == 1 );
      }
      else
      {
	 TEST( bp.indices()[0] == 1 );
	 TEST( bp.partners()[0] == 0 );
      }
   }
   else if( comm::world.size() == 3 )
   {
      if( comm::world.rank() == 0 )
      {
	 TEST( bp.displs().size() == 1 );
	 TEST( bp.displs()[0] == 0 );
	 TEST( bp.indices().size() == 0 );
	 TEST( bp.partners().size() == 0 );
      }
      else
      {
	 TEST( bp.displs().size() == 2 );
	 TEST( bp.displs()[0] == 0 );
	 TEST( bp.displs()[1] == 1 );
	 TEST( bp.indices().size() == 1 );
	 TEST( bp.partners().size() == 1 );
	 if( comm::world.rank() == 1 )
	 {
	    TEST( bp.indices()[0] == 0 );
	    TEST( bp.partners()[0] == 2 );
	 }
	 else
	 {
	    TEST( bp.indices()[0] == 1 );
	    TEST( bp.partners()[0] == 1 );
	 }
      }
   }
}

TEST_CASE( "construct/stationary" )
{
   if( comm::world.size() == 3 )
   {
      bpart bp;
      std::vector<int> part( 5 );
      if( comm::world.rank() == 0 )
      {
	 part[0] = 0;
	 part[1] = 1;
	 part[2] = 1;
	 part[3] = 0;
	 part[4] = 1;
      }
      else if( comm::world.rank() == 1 )
      {
	 part[0] = 0;
	 part[1] = 0;
	 part[2] = 0;
	 part[3] = 0;
	 part[4] = 0;
      }
      else
      {
	 part[0] = 0;
	 part[1] = 1;
	 part[2] = 1;
	 part[3] = 0;
	 part[4] = 0;
      }
      bp.construct( part.begin(), part.end() );
      if( comm::world.rank() == 0 )
      {
	 TEST( bp.displs().size() == 2 );
	 TEST( bp.displs()[0] == 0 );
	 TEST( bp.displs()[1] == 2 );
	 TEST( bp.indices().size() == 2 );
	 TEST( bp.indices()[0] == 0 );
	 TEST( bp.indices()[1] == 3 );
	 TEST( bp.partners().size() == 1 );
	 TEST( bp.partners()[0] == 2 );
      }
      else if( comm::world.rank() == 1 )
      {
      	 TEST( bp.displs().size() == 1 );
      	 TEST( bp.displs()[0] == 0 );
      	 TEST( bp.indices().size() == 0 );
      	 TEST( bp.partners().size() == 0 );
      }
      else
      {
      	 TEST( bp.displs().size() == 2 );
      	 TEST( bp.displs()[0] == 0 );
      	 TEST( bp.displs()[1] == 2 );
      	 TEST( bp.indices().size() == 2 );
      	 TEST( bp.indices()[0] == 1 );
      	 TEST( bp.indices()[1] == 2 );
      	 TEST( bp.partners().size() == 1 );
      	 TEST( bp.partners()[0] == 0 );
      }
   }
}

TEST_CASE( "construct/all_ranks_transfer" )
{
   if( comm::world.size() == 3 )
   {
      bpart bp;
      std::vector<int> part( 5 );
      if( comm::world.rank() == 0 )
      {
	 part[0] = 1;
	 part[1] = 0;
	 part[2] = 0;
	 part[3] = 0;
	 part[4] = 0;
      }
      else if( comm::world.rank() == 1 )
      {
	 part[0] = 0;
	 part[1] = 1;
	 part[2] = 1;
	 part[3] = 0;
	 part[4] = 1;
      }
      else
      {
	 part[0] = 0;
	 part[1] = 0;
	 part[2] = 0;
	 part[3] = 1;
	 part[4] = 0;
      }
      bp.construct( part.begin(), part.end() );
      if( comm::world.rank() == 0 )
      {
	 TEST( bp.displs().size() == 2 );
	 TEST( bp.displs()[0] == 0 );
	 TEST( bp.displs()[1] == 1 );
	 TEST( bp.indices().size() == 1 );
	 TEST( bp.indices()[0] == 0 );
	 TEST( bp.partners().size() == 1 );
	 TEST( bp.partners()[0] == 1 );
      }
      else if( comm::world.rank() == 1 )
      {
      	 TEST( bp.displs().size() == 3 );
	 TEST( bp.displs()[0] == 0 );
	 TEST( bp.displs()[1] == 1 );
	 TEST( bp.displs()[2] == 2 );
	 TEST( bp.indices().size() == 2 );
	 TEST( bp.indices()[0] == 0 );
	 TEST( bp.indices()[1] == 3 );
	 TEST( bp.partners().size() == 2 );
	 TEST( bp.partners()[0] == 2 );
	 TEST( bp.partners()[1] == 0 );
      }
      else
      {
      	 TEST( bp.displs().size() == 2 );
      	 TEST( bp.displs()[0] == 0 );
      	 TEST( bp.displs()[1] == 1 );
      	 TEST( bp.indices().size() == 1 );
      	 TEST( bp.indices()[0] == 3 );
      	 TEST( bp.partners().size() == 1 );
      	 TEST( bp.partners()[0] == 1 );
      }
   }
}

TEST_CASE( "construct/transfer/serial" )
{
   bpart bp( comm::self );
   std::vector<int> part( 5 ), data( 5 );
   part[0] = 0;
   part[1] = 1;
   part[2] = 0;
   part[3] = 1;
   part[4] = 0;
   bp.construct( part.begin(), part.end() );
   boost::iota( data, 0 );
   bp.transfer( data );
   TEST( data[0] == 0 );
   TEST( data[1] == 2 );
   TEST( data[2] == 4 );
   TEST( data[3] == 3 );
   TEST( data[4] == 1 );
}

TEST_CASE( "construct/transfer/parallel" )
{
   if( comm::world.size() == 3 )
   {
      bpart bp;
      std::vector<int> part( 5 ), data( 5 );
      if( comm::world.rank() == 0 )
      {
	 part[0] = 1;
	 part[1] = 0;
	 part[2] = 0;
	 part[3] = 0;
	 part[4] = 0;
      }
      else if( comm::world.rank() == 1 )
      {
	 part[0] = 0;
	 part[1] = 1;
	 part[2] = 1;
	 part[3] = 0;
	 part[4] = 1;
      }
      else
      {
	 part[0] = 0;
	 part[1] = 0;
	 part[2] = 0;
	 part[3] = 1;
	 part[4] = 0;
      }
      bp.construct( part.begin(), part.end() );
      boost::iota( data, 5*comm::world.rank() );
      bp.transfer( data );
      if( comm::world.rank() == 0 )
      {
	 TEST( data[0] == 8 );
	 TEST( data[1] == 1 );
	 TEST( data[2] == 2 );
	 TEST( data[3] == 3 );
	 TEST( data[4] == 4 );
      }
      else if( comm::world.rank() == 1 )
      {
	 TEST( data[0] == 13 );
	 TEST( data[1] == 6 );
	 TEST( data[2] == 7 );
	 TEST( data[3] == 0 );
	 TEST( data[4] == 9 );
      }
      else
      {
	 TEST( data[0] == 10 );
	 TEST( data[1] == 11 );
	 TEST( data[2] == 12 );
	 TEST( data[3] == 5 );
	 TEST( data[4] == 14 );
      }
   }
}
