#ifndef mpi_logger_hh
#define mpi_logger_hh

#include <typeinfo>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <mpi.h>
#include "libhpc/logging/logging.hh"

#ifndef NLOG

namespace hpc {
   namespace mpi {

      extern double log_base_time;

      ///
      ///
      ///
      class logger
         : public logging::file
      {
      public:

         logger( const std::string& filename );

         ~logger();

         virtual void
         open();

         virtual void
         prefix();

      protected:

         int _my_rank;
         std::string _base;
      };
   }
}

#endif

#endif
