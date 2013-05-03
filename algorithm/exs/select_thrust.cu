#include <thrust/device_vector.h>
#include <thrust/iterator/counting_iterator.h>
#include <libhpc/algorithm/select.hh>
#include <libhpc/algorithm/thrust/select.hh>
#include <libhpc/profile/timer.hh>

void
run_thrust( size_t size,
            const hpc::mpi::comm& comm )
{
   // Make a device vector to test with, fill it with
   // sequential information for now.
   thrust::device_vector<float> vec( size );
   thrust::copy(
      thrust::counting_iterator<float>( comm.rank()*size ),
      thrust::counting_iterator<float>( (comm.rank() + 1)*size ),
      vec.begin()
      );

   // Calculate the selection while timing.
   hpc::profile::timer timer;
   comm.barrier();
   timer.start();
   float x = hpc::algorithm::thrust::select<float>( vec.begin(), vec.end(), (size*comm.size())/3, comm );
   comm.barrier();
   timer.stop();
   if( comm.rank() == 0 )
   {
      std::cout << "Thrust selection value is " << x << "\n";
      std::cout << "Took " << timer.total() << " seconds.\n";
   }
}

void
run_host( size_t size,
          const hpc::mpi::comm& comm )
{
   // Make a device vector to test with, fill it with
   // sequential information for now.
   hpc::vector<float> vec( size );
   hpc::iota( vec.begin(), vec.end(), comm.rank()*size );

   // Calculate the selection while timing.
   hpc::profile::timer timer;
   comm.barrier();
   timer.start();
   float x = hpc::algorithm::select( vec.begin(), vec.end(), (size*comm.size())/3, comm );
   comm.barrier();
   timer.stop();
   if( comm.rank() == 0 )
   {
      std::cout << "Host selection value is " << x << "\n";
      std::cout << "Took " << timer.total() << " seconds.\n";
   }
}

void
application()
{
   hpc::mpi::comm comm( hpc::mpi::comm::world );
   if( comm.rank() == 0 )
      std::cout << "Launching with " << comm.size() << " rank(s).\n";

   // How big is this test?
   size_t size = 10000000;

   // Run both variants.
   run_host( size, comm );
   run_thrust( size, comm );
}

int
main( int argc,
      char* argv[] )
{
   hpc::mpi::initialise( argc, argv );
   application();
   hpc::mpi::finalise();
   return EXIT_SUCCESS;
}
