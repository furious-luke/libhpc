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

#include "globals.hh"
#include "instrument.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      stack _stack;

      void
      push( logger* log )
      {
         _stack.push( log );
      }

      void
      pop()
      {
         _stack.pop();
      }

      void
      clear()
      {
         _stack.clear();
      }

      void
      push_tag( const std::string& tag )
      {
         for( stack::iterator it = _stack.begin(); it != _stack.end(); ++it )
	    (*it)->push_tag( tag );
      }

      void
      pop_tag( const std::string& tag )
      {
         for( stack::iterator it = _stack.begin(); it != _stack.end(); ++it )
	    (*it)->pop_tag( tag );
      }

   }
}

#endif

#ifndef NINSTRUMENTATION

namespace hpc {
   namespace logging {

      // instrument instr;

   }
}

#endif
