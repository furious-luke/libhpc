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
#include "../fixtures/kdtree_fixture.hh"

SUITE_PREFIX( "/hpc/algorithm/kdtree/" );

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
   kdtree_fixture fix;

   TEST( fix.kdt.n_dims() == 3 );
   TEST( fix.kdt.n_cells() == 9 );
   TEST( fix.kdt.n_leafs() == 5 );
   TEST( fix.kdt.n_branches() == 4 );
   TEST( fix.kdt.bounds().size() == 3 );
   if( rank == 0 )
      TEST( fix.kdt.bounds()[0][0] == 1.0 );
   else
      DELTA( fix.kdt.bounds()[0][0], fix.base, 1.0 );
   if( rank == n_ranks - 1 )
      TEST( fix.kdt.bounds()[0][1] == 12.0 + fix.base );
   else
      DELTA( fix.kdt.bounds()[0][1], fix.base + 13.0, 1.0 );
   TEST( fix.kdt.bounds()[1][0] == 1.0 ); TEST( fix.kdt.bounds()[1][1] == 11.0 );
   TEST( fix.kdt.bounds()[2][0] == 1.0 ); TEST( fix.kdt.bounds()[2][1] == 10.0 );

   TEST( fix.bp.n_elems() == 5 );
   TEST( fix.bp.n_gelems() == 5*n_ranks );

   DELTA( fix.crds[0][0],  1.0 + fix.base, 1e-1 ); DELTA( fix.crds[1][0],  1.0, 1e-1 ); DELTA( fix.crds[2][0],  1.0, 1e-1 );
   DELTA( fix.crds[0][1],  2.0 + fix.base, 1e-1 ); DELTA( fix.crds[1][1],  2.0, 1e-1 ); DELTA( fix.crds[2][1], 10.0, 1e-1 );
   DELTA( fix.crds[0][2],  3.0 + fix.base, 1e-1 ); DELTA( fix.crds[1][2], 11.0, 1e-1 ); DELTA( fix.crds[2][2],  2.0, 1e-1 );
   DELTA( fix.crds[0][3], 11.0 + fix.base, 1e-1 ); DELTA( fix.crds[1][3],  4.0, 1e-1 ); DELTA( fix.crds[2][3],  2.5, 1e-1 );
   DELTA( fix.crds[0][4], 12.0 + fix.base, 1e-1 ); DELTA( fix.crds[1][4],  8.0, 1e-1 ); DELTA( fix.crds[2][4],  3.0, 1e-1 );
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

TEST_CASE( "iterator/next" )
{
   unsigned n_ranks = comm::world.size();
   unsigned rank = comm::world.rank();
   kdtree_fixture fix;

   auto it = fix.kdt.begin();
   TEST( it.cell() == 0 );
   ++it; TEST( it.cell() == 1 );
   ++it; TEST( it.cell() == 3 );
   ++it; TEST( it.cell() == 7 );
   ++it; TEST( it.cell() == 8 );
   ++it; TEST( it.cell() == 4 );
   ++it; TEST( it.cell() == 2 );
   ++it; TEST( it.cell() == 5 );
   ++it; TEST( it.cell() == 6 );
   ++it; TEST( it.cell() == 9 );
}

TEST_CASE( "iterator/skip" )
{
   unsigned n_ranks = comm::world.size();
   unsigned rank = comm::world.rank();
   kdtree_fixture fix;

   auto it = fix.kdt.begin();
   TEST( it.cell() == 0 );
   ++it;      TEST( it.cell() == 1 );
   ++it;      TEST( it.cell() == 3 );
   it.skip(); TEST( it.cell() == 4 );
   ++it;      TEST( it.cell() == 2 );
   it.skip(); TEST( it.cell() == 9 );
}

TEST_CASE( "iterator/bounds" )
{
   unsigned n_ranks = comm::world.size();
   unsigned rank = comm::world.rank();

   if( n_ranks == 1 )
   {
      kdtree_fixture fix;

      auto it = fix.kdt.begin();
      TEST( it.bounds()[0][0] == 1.0 ); TEST( it.bounds()[0][1] == 12.0 );
      TEST( it.bounds()[1][0] == 1.0 ); TEST( it.bounds()[1][1] == 11.0 );
      TEST( it.bounds()[2][0] == 1.0 ); TEST( it.bounds()[2][1] == 10.0 );
      ++it;
      TEST( it.bounds()[0][0] == 1.0 ); TEST( it.bounds()[0][1] == 6.5 );
      TEST( it.bounds()[1][0] == 1.0 ); TEST( it.bounds()[1][1] == 11.0 );
      TEST( it.bounds()[2][0] == 1.0 ); TEST( it.bounds()[2][1] == 10.0 );
      ++it;
      TEST( it.bounds()[0][0] == 1.0 ); TEST( it.bounds()[0][1] == 6.5 );
      TEST( it.bounds()[1][0] == 1.0 ); TEST( it.bounds()[1][1] == 6.0 );
      TEST( it.bounds()[2][0] == 1.0 ); TEST( it.bounds()[2][1] == 10.0 );
      ++it;
      TEST( it.bounds()[0][0] == 1.0 ); TEST( it.bounds()[0][1] == 6.5 );
      TEST( it.bounds()[1][0] == 1.0 ); TEST( it.bounds()[1][1] == 6.0 );
      TEST( it.bounds()[2][0] == 1.0 ); TEST( it.bounds()[2][1] == 5.5 );
      ++it;
      TEST( it.bounds()[0][0] == 1.0 ); TEST( it.bounds()[0][1] == 6.5 );
      TEST( it.bounds()[1][0] == 1.0 ); TEST( it.bounds()[1][1] == 6.0 );
      TEST( it.bounds()[2][0] == 5.5 ); TEST( it.bounds()[2][1] == 10.0 );
      ++it;
      TEST( it.bounds()[0][0] == 1.0 ); TEST( it.bounds()[0][1] == 6.5 );
      TEST( it.bounds()[1][0] == 6.0 ); TEST( it.bounds()[1][1] == 11.0 );
      TEST( it.bounds()[2][0] == 1.0 ); TEST( it.bounds()[2][1] == 10.0 );
      ++it;
      TEST( it.bounds()[0][0] == 6.5 ); TEST( it.bounds()[0][1] == 12.0 );
      TEST( it.bounds()[1][0] == 1.0 ); TEST( it.bounds()[1][1] == 11.0 );
      TEST( it.bounds()[2][0] == 1.0 ); TEST( it.bounds()[2][1] == 10.0 );
      ++it;
      TEST( it.bounds()[0][0] == 6.5 ); TEST( it.bounds()[0][1] == 12.0 );
      TEST( it.bounds()[1][0] == 1.0 ); TEST( it.bounds()[1][1] == 4.0 );
      TEST( it.bounds()[2][0] == 1.0 ); TEST( it.bounds()[2][1] == 10.0 );
      ++it;
      TEST( it.bounds()[0][0] == 6.5 ); TEST( it.bounds()[0][1] == 12.0 );
      TEST( it.bounds()[1][0] == 4.0 ); TEST( it.bounds()[1][1] == 11.0 );
      TEST( it.bounds()[2][0] == 1.0 ); TEST( it.bounds()[2][1] == 10.0 );
   }
}
