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

#ifndef hpc_system_process_hh
#define hpc_system_process_hh

#ifndef DARWIN

#include <sys/ptrace.h>
#include "file_descriptor.hh"

namespace hpc {
   namespace os {

      class process
      {
      public:

         process( pid_t pid = -1 );

	 int
	 wait( int* stat = nullptr,
	       int opts = 0 );

	 void
	 ptrace( enum __ptrace_request req,
		 void* addr = nullptr,
		 void* data = nullptr ) const;

         bool
         operator<( process const& op ) const;

      protected:

	 pid_t _pid;
      };

   }
}

#endif

#endif
