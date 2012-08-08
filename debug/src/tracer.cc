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

#include <stdio.h>
#include <string.h>
#include "tracer.hh"
#include "assert.hh"

namespace debug {

   tracer::tracer()
      : _num_entries(0),
	_pos(0)
   {
   }

   tracer::~tracer() {
   }

   void
   tracer::clear()
   {
      this->_num_entries = 0;
      this->_pos = 0;
   }

   void
   tracer::push( const char* func )
   {
      ASSERT(func && func[0] != 0, "Invalid function string name.");
      ASSERT(strlen(func) + this->_pos < this->max_size, "Stack size too small for function trace.");
      strcpy(this->_stack + this->_pos, func);
      this->_pos += strlen(func) + 1;
      ++this->_num_entries;
   }

   void
   tracer::pop()
   {
      ASSERT(this->_num_entries, "No entries in stack trace to pop.");
      ASSERT(this->_pos > 0, "Corrupt function stack.");

      // Find the end of the previous entry.
      --this->_pos;
      while(this->_pos > 0 && this->_stack[this->_pos - 1] != 0)
	 --this->_pos;

      --this->_num_entries;
   }

   int
   tracer::buffer_size( int indent ) const
   {
      return this->_num_entries*indent + this->_pos;
   }

   int
   tracer::print( int indent ) const
   {
      int pos = 0, offs = 0;
      for(int ii = 0; ii < this->_num_entries; ++ii) {
   	 for(int jj = 0; jj < indent; ++jj)
   	    printf(" ");
   	 offs += indent;
   	 offs += printf("%s\n", this->_stack + pos);
   	 pos += strlen(this->_stack + pos) + 1;
      }
      return offs;
   }

   int
   tracer::sprint( char* buf,
		   int indent ) const
   {
      int pos = 0, offs = 0;
      for(int ii = 0; ii < this->_num_entries; ++ii) {
   	 for(int jj = 0; jj < indent; ++jj)
   	    sprintf(buf + offs + jj, " ");
   	 offs += indent;
   	 offs += sprintf(buf + offs, "%s\n", this->_stack + pos);
   	 pos += strlen(this->_stack + pos) + 1;
      }
      return offs;
   }

   std::ostream&
   operator<<( std::ostream& strm,
	       const tracer& obj )
   {
      static const int indent = 2;
      int pos = 0, offs = 0;
      for(int ii = 0; ii < obj._num_entries; ++ii) {
	 for(int jj = 0; jj < indent; ++jj)
	    strm << " ";
	 strm << obj._stack + pos << "\n";
	 pos += strlen(obj._stack + pos) + 1;
      }
   }
}
