#include "logger.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      ///
      ///
      ///
      class file
         : public logger
      {
      public:

         file( const std::string& filename );

         ~file();

         virtual void
         open();

         virtual void
         close();

         virtual void
         new_line();

      protected:

         std::string _filename;
         std::ofstream _file;
      };
   }
}

#endif
