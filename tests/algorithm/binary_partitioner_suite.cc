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
#include <libhpc/algorithm/binary_partitioner.hh>

typedef hpc::mpi::comm comm;

SUITE_PREFIX( "/hpc/algorithm/binary_partitioner/" );

struct permute
{
   permute( std::array<std::vector<double>,3>& crds )
      : crds( crds )
   {
   }

   void
   operator()( hpc::mpi::balanced_partition const& part )
   {
      for( unsigned ii = 0; ii < 3; ++ii )
         part.transfer( crds[ii] );
   }

   std::array<std::vector<double>,3>& crds;
};

TEST_CASE( "construct" )
{
   std::array<std::vector<double>,3> crds;
   for( unsigned ii = 0; ii < 3; ++ii )
   {
      for( unsigned jj = 0; jj < 10; ++jj )
	 crds[ii].push_back( 10*(ii + 1)*(jj + 1) );
   }
   permute perm( crds );
   auto bp = hpc::make_binary_partitioner( crds.begin(), crds.end(), perm, 1 );
   TEST( bp.n_elems() == 10 );
   TEST( bp.n_gelems() == comm::world.size()*10 );
   TEST( bp.n_leaf_cells() == 10 );
   TEST( bp.depth() == 4 );
   TEST( bp.n_incomplete_leaf_cells() == 4 );
   TEST( bp.n_cells() == 19 );
}

TEST_CASE( "partition/serial" )
{
   std::array<std::vector<double>,3> crds;
   for( unsigned ii = 0; ii < 3; ++ii )
   {
      for( unsigned jj = 0; jj < 10; ++jj )
	 crds[ii].push_back( 10*(ii + 1)*(jj + 1) );
      std::reverse( crds[ii].begin(), crds[ii].end() );
   }
   std::vector<std::array<double,2> > bnds( 3 );
   bnds[0] = std::array<double,2>{ 10.0, 100.0 };
   bnds[1] = std::array<double,2>{ 20.0, 200.0 };
   bnds[2] = std::array<double,2>{ 30.0, 300.0 };
   permute perm( crds );
   auto bp = hpc::make_binary_partitioner( crds.begin(), crds.end(), perm, 1 );

   bp.partition( 0, bnds, comm::self );
   if( comm::world.size() == 1 )
   {
      TEST( bp.offsets()[0] == 0 );
      TEST( bp.offsets()[1] == 0 );
      TEST( bp.offsets()[2] == 5 );
      TEST( bp.counts()[0] == 10 );
      TEST( bp.counts()[1] == 5 );
      TEST( bp.counts()[2] == 5 );
      for( unsigned ii = 0; ii < 5; ++ii )
      {
         hpc::view<std::vector<double> > cur_crds( crds[0], bp.counts()[1], bp.offsets()[1] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 10*(ii + 1) ) != cur_crds.end()) == true );
         cur_crds.assign( crds[1], bp.counts()[1], bp.offsets()[1] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 20*(ii + 1) ) != cur_crds.end()) == true );
         cur_crds.assign( crds[2], bp.counts()[1], bp.offsets()[1] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 30*(ii + 1) ) != cur_crds.end()) == true );
      }
      for( unsigned ii = 5; ii < 10; ++ii )
      {
         hpc::view<std::vector<double> > cur_crds( crds[0], bp.counts()[2], bp.offsets()[2] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 10*(ii + 1) ) != cur_crds.end()) == true );
         cur_crds.assign( crds[1], bp.counts()[2], bp.offsets()[2] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 20*(ii + 1) ) != cur_crds.end()) == true );
         cur_crds.assign( crds[2], bp.counts()[2], bp.offsets()[2] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 30*(ii + 1) ) != cur_crds.end()) == true );
      }
      TEST( std::get<1>( bp.split() ) == 2 );
      DELTA( std::get<0>( bp.split() ), 165.0, 0.1 );
      TEST( bp.sub_comm().size() == 1 );
   }

   bnds[2][1] = std::get<0>( bp.split() );
   bp.partition( 1, bnds, comm::self );
   if( comm::world.size() == 1 )
   {
      TEST( bp.offsets()[0] == 0 );
      TEST( bp.offsets()[1] == 0 );
      TEST( bp.offsets()[2] == 5 );
      TEST( bp.offsets()[3] == 0 );
      TEST( bp.offsets()[4] == 3 );
      TEST( bp.counts()[0] == 10 );
      TEST( bp.counts()[1] == 5 );
      TEST( bp.counts()[2] == 5 );
      TEST( bp.counts()[3] == 3 );
      TEST( bp.counts()[4] == 2 );
      for( unsigned ii = 0; ii < 3; ++ii )
      {
         hpc::view<std::vector<double> > cur_crds( crds[0], bp.counts()[3], bp.offsets()[3] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 10*(ii + 1) ) != cur_crds.end()) == true );
         cur_crds.assign( crds[1], bp.counts()[3], bp.offsets()[3] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 20*(ii + 1) ) != cur_crds.end()) == true );
         cur_crds.assign( crds[2], bp.counts()[3], bp.offsets()[3] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 30*(ii + 1) ) != cur_crds.end()) == true );
      }
      for( unsigned ii = 3; ii < 5; ++ii )
      {
         hpc::view<std::vector<double> > cur_crds( crds[0], bp.counts()[4], bp.offsets()[4] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 10*(ii + 1) ) != cur_crds.end()) == true );
         cur_crds.assign( crds[1], bp.counts()[4], bp.offsets()[4] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 20*(ii + 1) ) != cur_crds.end()) == true );
         cur_crds.assign( crds[2], bp.counts()[4], bp.offsets()[4] );
         TEST( (std::find( cur_crds.begin(), cur_crds.end(), 30*(ii + 1) ) != cur_crds.end()) == true );
      }
      TEST( std::get<1>( bp.split() ) == 1 );
      DELTA( std::get<0>( bp.split() ), 60.0, 0.1 );
      TEST( bp.sub_comm().size() == 1 );
   }
}
