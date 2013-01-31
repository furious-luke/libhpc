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

#ifndef NSTACKTRACE

#include <stdlib.h>
#include <execinfo.h>
#include "stacktrace.hh"
#include "func.hh"

namespace hpc {
   namespace debug {

      const unsigned stacktrace::MAX_DEPTH = 32;

      stacktrace::stacktrace()
      {
         _get_stack();
      }

      stacktrace::const_iterator
      stacktrace::begin() const
      {
         return _st;
      }

      stacktrace::const_iterator
      stacktrace::end() const
      {
         return _st + _depth;
      }

      void
      stacktrace::_get_stack()
      {
         void* trace[MAX_DEPTH];
         _depth = backtrace( trace, MAX_DEPTH ) - 2;
         for( unsigned ii = 0; ii < _depth; ++ii )
         {
            func_details( trace[ii + 2], &_st[ii].file_name, &_st[ii].func_name );
            if( !_st[ii].file_name )
               _st[ii].file_name = "??";
            if( !_st[ii].func_name )
               _st[ii].func_name = (char*)"??";
         }
      }
   }
}

#endif
