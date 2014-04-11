#include <libhpc/system/application.hh>
#include <libhpc/system/daemon.hh>

class application
   : public hpc::application
{
public:

   application( int argc,
                char* argv[] )
      : hpc::application( argc, argv )
   {
      hpc::daemonise();
   }

   void
   operator()()
   {
      std::cout << "Hello world!\n";
   }
};

#include <libhpc/system/main.hh>
