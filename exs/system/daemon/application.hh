#ifndef ks_iotest_application_hh
#define ks_iotest_application_hh

#include <libhpc/main/application.hh>

   class application
      : public hpc::application
   {
   public:

      application( int argc,
		   char* argv[] );

      void
      operator()();
   };

#endif
