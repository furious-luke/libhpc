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

#ifndef DARWIN

#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "libhpc/debug/insist.hh"
#include "process.hh"

namespace hpc {
   namespace os {

      process::process( pid_t pid )
	 : _pid( pid )
      {
      }

      int
      process::wait( int* stat,
		     int opts )
      {
	 int res = waitpid( _pid, stat, opts );
	 ASSERT( res >= 0 );
	 return res;
      }

      void
      process::ptrace( enum __ptrace_request req,
		       void* addr,
		       void* data ) const
      {
	 // INSIST( ::ptrace( req, _pid, addr, data ), >= 0 );
	if( ::ptrace( req, _pid, addr, data ) < 0 ) {
	   std::cout << "ERROR: " << strerror( errno ) << "\n";
	   ASSERT( 0 );
	}
      }

      bool
      process::operator<( process const& op ) const
      {
	 return _pid < op._pid;
      }

   }
}

#endif
