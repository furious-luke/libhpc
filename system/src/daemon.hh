#ifndef libhpc_system_daemon_hh
#define libhpc_system_daemon_hh

namespace hpc {
   namespace system {

      bool
      daemonize( const std::string& root = "/root/" );
   }
}

#endif
