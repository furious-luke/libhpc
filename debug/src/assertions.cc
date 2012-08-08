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

#ifndef NDEBUG

namespace debug {

#ifndef NTRACESTACK
   extern tracer stack_trace;
#endif

   int log2i(int x)
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
	_file(NULL),
	_line(0),
	_expr(NULL)
   {
      if(msg)
	 strcpy(this->_msg, msg);
      else
	 this->_msg[0] = 0;
      this->_buf[0] = 0;
   }

   assertion::assertion( const assertion& ass )
      : exception(ass),
	_file(ass._file),
	_line(ass._line),
	_expr(ass._expr)
   {
      strcpy(this->_msg, ass._msg);
      strcpy(this->_buf, ass._buf);
   }

   assertion::~assertion() throw()
   {
   }

   void
   assertion::details( const char* file,
		       int line,
		       const char* expr ) throw()
   {
      this->_file = file;
      this->_line = line;
      this->_expr = expr;
      this->write_buffer();
   }

   const char*
   assertion::what() const throw()
   {
      return this->_buf;
   }

   void
   assertion::write_buffer() throw()
   {
      int offs = sprintf(this->_buf, "\n\nLocation:\n  %s: %d: %s\n", this->_file, this->_line, this->_expr);
#ifndef NTRACESTACK
      offs += sprintf(this->_buf + offs, "\nStack trace:\n");
      offs += stack_trace.sprint(this->_buf + offs, 2);
#endif
      if(this->_msg[0] != 0)
	 sprintf(this->_buf + offs, "\n%s\n", this->_msg);
   }
}

#endif
