#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "libhpc/debug/debug.hh"
#include "daemon.hh"

namespace hpc {
   namespace system {

      void
      signal_handler( int sig )
      {
         switch( sig )
         {
            case SIGHUP:
               break;
            case SIGTERM:
               exit( 0 );
               break;
         }
      }

      bool
      daemonize( const std::string& name,
                 const std::string& root )
      {
         // Fork to make a child.
         int res = fork();
         ASSERT( res >= 0 );

         // Detach from parent group.
         setsid();

         // Close all descriptors.
         for( int ii = getdtablesize(); ii >= 0; --ii )
            close( ii );

         // Pipe everything to null for now.
         // TODO: Pipe to log?
         int fd = open( "/dev/null", O_RDWR );
         dup( fd );
         dup( fd );

         // Make sure any new files are protected.
         umask( 027 );

         // Move to the appropriate root directory.
         chdir( root.c_str() );

         // Create a file lock to indicate the daemon is already running.
         int lfp = open( "daemon.lock", O_RDWR | O_CREAT, 0640 );
         ASSERT( lfp >= 0 );
         if( lockf( lfp, F_TLOCK, 0 ) < 0 )
         {
            // Failed to lock file, another instance is likely running.
            exit( 0 );
         }

         // Dump pid to lock file.
         char pid_str[20];
         sprintf( pid_str, "%d\n", getpid() );
         write( lfp, pid_str, strlen(pid_str) );

         // Handle various signals.
         signal( SIGCHLD, SIG_IGN );
         signal( SIGHUP, signal_handler );
         signal( SIGTERM, signal_handler );

         // Prepare logging using syslogd. TODO
         // openlog( name.c_str(), LOG_PID, LOG_DAEMON );
      }
   }
}
