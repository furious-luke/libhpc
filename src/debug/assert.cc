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
#include "assert.hh"

#if !defined( NDEBUG ) || !defined( NEXCEPT )

namespace hpc {
   namespace debug {

      extern bool use_abort;

      void
      _assert( bool state,
               const char* file,
               int line,
               const char* expr,
#ifndef NSTACKTRACE
               const stacktrace& st,
#endif
               const char* msg )
      {
         _assert(
            state, file, line, expr,
#ifndef NSTACKTRACE
            st,
#endif
            assertion( msg )
            );
      }

      void
      _assert( bool state,
               const char* file,
               int line,
               const char* expr,
#ifndef NSTACKTRACE
               const stacktrace& st,
#endif
               assertion asrt )
      {
         if( !state )
         {
            asrt.details(
               file, line, expr
#ifndef NSTACKTRACE
               , st
#endif
               );
            if( use_abort )
               abort();
            else
               throw asrt;
         }
      }
   }
}

#endif
