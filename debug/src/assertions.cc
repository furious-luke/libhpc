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
#include <string.h>
#include <stdio.h>
#include "assertions.hh"
#include "tracer.hh"

#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef NDEBUG

namespace hpc {
   namespace debug {

#ifndef NSTACKTRACE
      extern tracer stack_trace;
#endif

      int
      log2i( int x )
      {
         int ii = 0;
         while(x) {
            x >>= 1;
            ++ii;
         }
         return ii;
      }

      assertion::assertion( const char* msg ) throw()
         : exception(),
           _file( NULL ),
           _line( 0 ),
           _expr( NULL )
      {
         if( msg )
            strcpy( _msg, msg );
         else
            _msg[0] = 0;
         _buf[0] = 0;
      }

      assertion::assertion( const assertion& ass )
         : exception( ass ),
           _file( ass._file ),
           _line( ass._line ),
           _expr( ass._expr )
      {
         strcpy( _msg, ass._msg );
         strcpy( _buf, ass._buf );
      }

      assertion::~assertion() throw()
      {
      }

      void
      assertion::details( const char* file,
                          int line,
                          const char* expr ) throw()
      {
         _file = file;
         _line = line;
         _expr = expr;
         _write_buffer();
      }

      const char*
      assertion::what() const throw()
      {
         return _buf;
      }

      void
      assertion::_write_buffer() throw()
      {
         int offs = sprintf( _buf, "\n\nLocation:\n  %s: %d: %s\n", _file, _line, _expr );
#ifdef _OPENMP
	 offs += sprintf( _buf + offs, "Thread ID: %d\n", omp_get_thread_num() );
#endif
#ifndef NSTACKTRACE
         offs += sprintf( _buf + offs, "Stack trace:\n" );
         offs += stack_trace.sprint( _buf + offs, 2 );
#endif
         if( _msg[0] != 0 )
            sprintf( _buf + offs, "\n%s\n", _msg );
      }

      out_of_memory::out_of_memory()
         : assertion( "Out of memory." )
      {
      }

      invalid_index::invalid_index()
         : assertion( "Index out of bounds" )
      {
      }
   }
}

#endif
