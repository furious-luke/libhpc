#include <syslog.h>
#include "syslogd.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      syslogd::syslogd( const std::string& name )
         : logger(),
           _name( name )
      {
      }

      syslogd::~syslogd()
      {
      }

      void
      syslogd::new_line()
      {
         _buf << "\n";
         syslog( LOG_INFO, "%s", _buf.str().c_str() );
         _buf.str( std::string() );
         _new_line = true;
      }
   }
}

#endif
