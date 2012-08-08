// Copyright 2012 Luke Hodkinson

// This file is part of libhpc.
// 
// libhpc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// libhpc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with libhpc.  If not, see <http://www.gnu.org/licenses/>.

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
