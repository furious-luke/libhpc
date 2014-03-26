#include <iostream>
#include <libhpc/mpi/application.hh>
#include <libhpc/mpi/host.hh>
#include <libhpc/system/stream_output.hh>

class application
   : public hpc::mpi::application
{
public:

   application( int argc,
                char* argv[] )
      : hpc::mpi::application( argc, argv )
   {
   }

   void
   operator()()
   {
      std::set<int> ranks = hpc::mpi::make_host_ranks();
      std::cout << hpc::mpi::comm::world.rank() << ": " << ranks << "\n";
   }
};

#define HPC_APP_CLASS application
#include <libhpc/mpi/main.hh>
