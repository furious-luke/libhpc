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
#include <libhpc/algorithm/kdtree.hh>

typedef hpc::mpi::comm comm;

SUITE_PREFIX( "/hpc/algorithm/kdtree/" );

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
   if( comm::world.size() == 1 )
   {
      std::array<std::vector<double>,3> crds;
      for( unsigned ii = 0; ii < 3; ++ii )
      {
	 for( unsigned jj = 0; jj < 10; ++jj )
	    crds[ii].push_back( 10*(ii + 1)*(jj + 1) );
      }
      permute perm( crds );
      auto bp = hpc::make_binary_partitioner( crds.begin(), crds.end(), perm, 1 );
      hpc::kdtree<> kdt( bp );
   }
}

TEST_CASE( "construct/imbalanced" )
{
   if( comm::world.size() == 1 )
   {
      std::array<std::vector<double>,3> crds;
      for( unsigned ii = 0; ii < 3; ++ii )
      {
	 for( unsigned jj = 0; jj < 21; ++jj )
	    crds[ii].push_back( 10*(ii + 1)*(jj + 1) );
      }
      permute perm( crds );
      auto bp = hpc::make_binary_partitioner( crds.begin(), crds.end(), perm, 2 );
      hpc::kdtree<> kdt( bp );
   }
}

TEST_CASE( "construct/parallel" )
{
   unsigned n_ranks = comm::world.size();
   unsigned rank = comm::world.rank();
   std::array<std::vector<double>,3> crds;
   for( unsigned ii = 0; ii < 3; ++ii )
      crds[ii].reserve( 5 );

   crds[0].push_back(  1.0 ); crds[1].push_back(  1.0 ); crds[2].push_back(  1.0 );
   crds[0].push_back(  2.0 ); crds[1].push_back(  2.0 ); crds[2].push_back( 10.0 );
   crds[0].push_back(  3.0 ); crds[1].push_back( 11.0 ); crds[2].push_back(  2.0 );
   crds[0].push_back( 11.0 ); crds[1].push_back(  4.0 ); crds[2].push_back(  2.5 );
   crds[0].push_back( 12.0 ); crds[1].push_back(  8.0 ); crds[2].push_back(  3.0 );
   double base = 13.0*(double)rank;
   for( auto& x : crds[0] )
      x += base;

   permute perm( crds );
   auto bp = hpc::make_binary_partitioner( crds.begin(), crds.end(), perm, 1 );
   hpc::kdtree<> kdt( bp );

   TEST( kdt.n_dims() == 3 );
   TEST( kdt.n_cells() == 9 );
   TEST( kdt.n_leafs() == 5 );
   TEST( kdt.n_branches() == 4 );
   TEST( kdt.bounds().size() == 3 );
   if( rank == 0 )
      TEST( kdt.bounds()[0][0] == 1.0 );
   else
      DELTA( kdt.bounds()[0][0], base, 1.0 );
   if( rank == n_ranks - 1 )
      TEST( kdt.bounds()[0][1] == 12.0 + base );
   else
      DELTA( kdt.bounds()[0][1], base + 13.0, 1.0 );
   TEST( kdt.bounds()[1][0] == 1.0 ); TEST( kdt.bounds()[1][1] == 11.0 );
   TEST( kdt.bounds()[2][0] == 1.0 ); TEST( kdt.bounds()[2][1] == 10.0 );

   TEST( bp.n_elems() == 5 );
   TEST( bp.n_gelems() == 5*n_ranks );

   DELTA( crds[0][0],  1.0 + base, 1e-1 ); DELTA( crds[1][0],  1.0, 1e-1 ); DELTA( crds[2][0],  1.0, 1e-1 );
   DELTA( crds[0][1],  2.0 + base, 1e-1 ); DELTA( crds[1][1],  2.0, 1e-1 ); DELTA( crds[2][1], 10.0, 1e-1 );
   DELTA( crds[0][2],  3.0 + base, 1e-1 ); DELTA( crds[1][2], 11.0, 1e-1 ); DELTA( crds[2][2],  2.0, 1e-1 );
   DELTA( crds[0][3], 11.0 + base, 1e-1 ); DELTA( crds[1][3],  4.0, 1e-1 ); DELTA( crds[2][3],  2.5, 1e-1 );
   DELTA( crds[0][4], 12.0 + base, 1e-1 ); DELTA( crds[1][4],  8.0, 1e-1 ); DELTA( crds[2][4],  3.0, 1e-1 );
}

TEST_CASE( "leaf_geometry/depth_1" )
{
   if( comm::world.size() == 1 )
   {
      std::array<std::vector<double>,3> crds;
      crds[0].resize( 2 );
      crds[0][0] = 0.0;
      crds[0][1] = 10.0;
      crds[1].resize( 2 );
      crds[1][0] = 0.0;
      crds[1][1] = 1.0;
      crds[2].resize( 2 );
      crds[2][0] = 0.0;
      crds[2][1] = 1.0;
      permute perm( crds );
      auto bp = hpc::make_binary_partitioner( crds.begin(), crds.end(), perm, 1 );
      hpc::kdtree<> kdt( bp );
      TEST( kdt.n_leafs() == 2 );
      TEST( kdt.n_dims() == 3 );
      TEST( kdt.n_branches() == 1 );
      TEST( kdt.n_cells() == 3 );
      TEST( kdt.splits().size() == 1 );
      TEST( kdt.splits()[0].dim == 0 );
      DELTA( kdt.splits()[0].pos, 5.0, 1e-2 );
      DELTA( crds[0][0], 0.0, 1e-2 );
      DELTA( crds[0][1], 10.0, 1e-2 );

      std::array<double,6> bnds;
      kdt.leaf_geometry( 0, bnds.begin() );
      DELTA( bnds[0], 0.0, 1e-2 );
      DELTA( bnds[1], 5.0, 1e-2 );
      DELTA( bnds[2], 0.0, 1e-2 );
      DELTA( bnds[3], 1.0, 1e-2 );
      DELTA( bnds[4], 0.0, 1e-2 );
      DELTA( bnds[5], 1.0, 1e-2 );
      kdt.leaf_geometry( 1, bnds.begin() );
      DELTA( bnds[0], 5.0, 1e-2 );
      DELTA( bnds[1], 10.0, 1e-2 );
      DELTA( bnds[2], 0.0, 1e-2 );
      DELTA( bnds[3], 1.0, 1e-2 );
      DELTA( bnds[4], 0.0, 1e-2 );
      DELTA( bnds[5], 1.0, 1e-2 );
   }
}

TEST_CASE( "leaf_geometry/depth_2" )
{
   if( comm::world.size() == 1 )
   {
      std::array<std::vector<double>,3> crds;
      crds[0].resize( 3 );
      crds[0][0] = 0.0;
      crds[0][1] = 10.0;
      crds[0][2] = 0.0;
      crds[1].resize( 3 );
      crds[1][0] = 0.0;
      crds[1][1] = 1.0;
      crds[1][2] = 1.0;
      crds[2].resize( 3 );
      crds[2][0] = 1.0;
      crds[2][1] = 0.0;
      crds[2][2] = 7.0;
      permute perm( crds );
      auto bp = hpc::make_binary_partitioner( crds.begin(), crds.end(), perm, 1 );
      hpc::kdtree<> kdt( bp );
      TEST( kdt.n_leafs() == 3 );
      TEST( kdt.n_dims() == 3 );
      TEST( kdt.n_branches() == 2 );
      TEST( kdt.n_cells() == 5 );
      TEST( kdt.splits().size() == 2 );
      TEST( kdt.splits()[0].dim == 0 );
      DELTA( kdt.splits()[0].pos, 5.0, 1e-2 );
      TEST( kdt.splits()[1].dim == 2 );
      DELTA( kdt.splits()[1].pos, 4.0, 1e-2 );

      std::array<double,6> bnds;
      kdt.leaf_geometry( 0, bnds.begin() );
      DELTA( bnds[0], 0.0, 1e-2 );
      DELTA( bnds[1], 5.0, 1e-2 );
      DELTA( bnds[2], 0.0, 1e-2 );
      DELTA( bnds[3], 1.0, 1e-2 );
      DELTA( bnds[4], 0.0, 1e-2 );
      DELTA( bnds[5], 4.0, 1e-2 );
      kdt.leaf_geometry( 1, bnds.begin() );
      DELTA( bnds[0], 0.0, 1e-2 );
      DELTA( bnds[1], 5.0, 1e-2 );
      DELTA( bnds[2], 0.0, 1e-2 );
      DELTA( bnds[3], 1.0, 1e-2 );
      DELTA( bnds[4], 4.0, 1e-2 );
      DELTA( bnds[5], 7.0, 1e-2 );
      kdt.leaf_geometry( 2, bnds.begin() );
      DELTA( bnds[0], 5.0, 1e-2 );
      DELTA( bnds[1], 10.0, 1e-2 );
      DELTA( bnds[2], 0.0, 1e-2 );
      DELTA( bnds[3], 1.0, 1e-2 );
      DELTA( bnds[4], 0.0, 1e-2 );
      DELTA( bnds[5], 7.0, 1e-2 );
   }
}
