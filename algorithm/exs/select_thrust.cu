#include <libhpc/algorithm/select.hh>
#include <libhpc/profile/timer.hh>
#include <thrust/device_vector.h>
#include <thrust/iterator/counting_iterator.h>

using namespace hpc;

void
application()
{
   mpi::comm comm( mpi::comm::world );
   if( comm.rank() == 0 )
      std::cout << "Launching with " << comm.size() << " rank(s).\n";

   // How big is this test?
   size_t size = 10000;

   // Make a device vector to test with, fill it with
   // sequential information for now.
   thrust::device_vector<float> vec( size );
   thrust::copy(
      thrust::counting_iterator<float>( comm.rank()*size ),
      thrust::counting_iterator<float>( (comm.rank() + 1)*size ),
      vec.begin()
      );

   // Calculate the selection while timing.
   profile::timer timer;
   comm.barrier();
   timer.start();
   float x = algorithm::select_thrust<float>( vec.begin(), vec.end(), (size*comm.size())/2, comm );
   comm.barrier();
   timer.stop();
   if( comm.rank() == 0 )
   {
      std::cout << "Selection value is " << x << "\n";
      std::cout << "Took " << timer.total() << " seconds.\n";
   }
}

int
main( int argc,
      char* argv[] )
{
   mpi::initialise( argc, argv );
   application();
   mpi::finalise();
   return EXIT_SUCCESS;
}
