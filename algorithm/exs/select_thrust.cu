#include <libhpc/algorithm/select.hh>

using namespace hpc;

int
main( int argc,
      char* argv[] )
{
   mpi::initialise( argc, argv );
   mpi::comm comm( mpi::comm::world );
   std::cout << "Launching with " << comm.size() << " rank(s).\n";

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
   float x = algorithm::select( vec.begin(), vec.end(), (size*comm.size())/2, comm );
   comm.barrier();
   timer.stop();
   std::cout << "Selection value is " << x << "\n";
   std::cout << "Took " << timer.total() << " seconds.\n";

   // mpi::finalise();
   return EXIT_SUCCESS;
}
