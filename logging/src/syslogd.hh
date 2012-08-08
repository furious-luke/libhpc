#include "logger.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      ///
      ///
      ///
      class syslogd
         : public logger
      {
      public:

         syslogd( const std::string& name );

         ~syslogd();

         virtual void
         new_line();

      protected:

         std::string _name;
      };
   }
}

#endif
