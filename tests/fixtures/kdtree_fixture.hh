#include <array>
#include <vector>
#include <libhpc/algorithm/binary_partitioner.hh>
#include <libhpc/algorithm/kdtree.hh>

typedef hpc::mpi::comm comm;

struct permute
{
   permute()
      : crds( nullptr )
   {
   }

   permute( std::array<std::vector<double>,3>& crds )
      : crds( &crds )
   {
   }

   void
   operator()( hpc::mpi::balanced_partition const& part )
   {
      for( unsigned ii = 0; ii < 3; ++ii )
         part.transfer( (*crds)[ii] );
   }

   std::array<std::vector<double>,3>* crds;
};

struct kdtree_fixture
{
   kdtree_fixture()
   {
      unsigned n_ranks = comm::world.size();
      unsigned rank = comm::world.rank();
      for( unsigned ii = 0; ii < 3; ++ii )
         crds[ii].reserve( 5 );

      crds[0].push_back(  1.0 ); crds[1].push_back(  1.0 ); crds[2].push_back(  1.0 );
      crds[0].push_back(  2.0 ); crds[1].push_back(  2.0 ); crds[2].push_back( 10.0 );
      crds[0].push_back(  3.0 ); crds[1].push_back( 11.0 ); crds[2].push_back(  2.0 );
      crds[0].push_back( 11.0 ); crds[1].push_back(  4.0 ); crds[2].push_back(  2.5 );
      crds[0].push_back( 12.0 ); crds[1].push_back(  8.0 ); crds[2].push_back(  3.0 );
      base = 13.0*(double)rank;
      for( auto& x : crds[0] )
         x += base;

      permute perm( crds );
      bp = hpc::make_binary_partitioner( crds.begin(), crds.end(), perm, 1 );
      kdt.construct( bp );
   }

   hpc::kdtree<> kdt;
   hpc::binary_partitioner<
      std::array<std::vector<double>,3>::iterator,
      permute> bp;
   std::array<std::vector<double>,3> crds;
   double base;
};
