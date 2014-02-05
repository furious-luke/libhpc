#include <libhpc/system/daemon.hh>
#include "application.hh"

application::application( int argc,
			  char* argv[] )
   : hpc::application( argc, argv )
{
   hpc::daemonise();
}

void
application::operator()()
{
   std::cout << "Hello world!\n";
}
