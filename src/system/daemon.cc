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
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace hpc {

   void
   daemonise()
   {
      // Fork the process and have the parent exit. If the process was started
      // from a shell, this returns control to the user. Forking a new process is
      // also a prerequisite for the subsequent call to setsid().
      if( pid_t pid = fork() )
      {
	 if( pid > 0 )
	 {
	    // We're in the parent process and need to exit.
	    exit( 0 );
	 }
	 else
	 {
	    syslog( LOG_ERR | LOG_USER, "First fork failed: %m" );
	    exit( 1 );
	 }
      }

      // Make the process a new session leader. This detaches it from the
      // terminal.
      setsid();

      // A process inherits its working directory from its parent. This could be
      // on a mounted filesystem, which means that the running daemon would
      // prevent this filesystem from being unmounted. Changing to the root
      // directory avoids this problem.
      chdir( "/" );

      // The file mode creation mask is also inherited from the parent process.
      // We don't want to restrict the permissions on files created by the
      // daemon, so the mask is cleared.
      umask( 0 );

      // A second fork ensures the process cannot acquire a controlling terminal.
      if( pid_t pid = fork() )
      {
	 if( pid > 0 )
	    exit(0);
	 else
	 {
	    syslog( LOG_ERR | LOG_USER, "Second fork failed: %m" );
	    exit( 1 );
	 }
      }

      // Close the standard streams. This decouples the daemon from the terminal
      // that started it.
      close( 0 );
      close( 1 );
      close( 2 );

      // We don't want the daemon to have any standard input.
      if( open( "/dev/null", O_RDONLY ) < 0 )
      {
	 syslog( LOG_ERR | LOG_USER, "Unable to open /dev/null: %m" );
	 exit( 1 );
      }

      // Send standard output to a log file.
      char const* output = tmpnam( 0 );
      int const flags = O_WRONLY | O_CREAT | O_APPEND;
      mode_t const mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
      if( open(output, flags, mode ) < 0 )
      {
	 syslog( LOG_ERR | LOG_USER, "Unable to open output file %s: %m", output );
	 exit( 1 );
      }

      // Also send standard error to the same log file.
      if( dup( 1 ) < 0 )
      {
	 syslog( LOG_ERR | LOG_USER, "Unable to dup output descriptor: %m" );
	 exit( 1 );
      }
   }

}
