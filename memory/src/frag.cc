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
#include "libhpc/system/id.hh"
#include "frag.hh"

namespace hpc {
   namespace memory {

#ifndef NMEMOPS

      class shutdown
      {
      public:
	 shutdown()
	    : _is_shutdown(false)
	 {
	 }

	 ~shutdown() {
	    log_ops();
	    this->_is_shutdown = true;
	 }

	 bool _is_shutdown;
      };

      struct memory_operation {
	 bool alloc;
	 unsigned long timestamp;
	 const void* begin;
	 const void* end;
      };

      const size_t max_ops = 16384;
      size_t num_ops = 0;
      memory_operation ops[max_ops];
      shutdown _shutdown;

      void
      add_op( bool alloc,
	      const void* begin,
	      const void* end )
      {
	 ops[num_ops].alloc = alloc;
	 ops[num_ops].begin = begin;
	 ops[num_ops].end = end;
	 ++num_ops;
	 if(num_ops == max_ops || _shutdown._is_shutdown)
	    log_ops();
      }

      void
      log_ops()
      {
	 if(num_ops) {

	    // Create a filename using the process ID.
	    long id = os::get_process_id();
	    char filename[1000];
	    sprintf(filename, "memory_ops.%ld.txt", id);

	    FILE* file = fopen(filename, "a");
	    for(size_t ii = 0; ii < num_ops; ++ii)
	       fprintf(file, "%s %p %p\n", ops[ii].alloc ? "alloc" : "free", ops[ii].begin, ops[ii].end);
	    fclose(file);
	    num_ops = 0;
	 }
      }

#endif

   }
}
