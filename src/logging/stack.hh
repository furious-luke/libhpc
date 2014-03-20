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

#ifndef libhpc_logging_stack_hh
#define libhpc_logging_stack_hh

#include <list>
#include <memory>
#include "libhpc/memory/memory.hh"
#include "logger.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      ///
      ///
      ///
      class stack
      {
      public:

	typedef std::list<std::unique_ptr<logger> >::iterator iterator;

      public:

         stack();

         ~stack();

         void
         push( logger* log );

         void
         pop();

         void
         clear();

         iterator
	 begin();

         iterator
	 end();

         template< class T >
         stack&
         operator<<( const T& obj )
         {
	    for( iterator it = _logs.begin(); it != _logs.end(); ++it )
	    {
	       std::unique_ptr<logger>& log = *it;
               *log << obj;
            }
            return *this;
         }

      protected:

	 std::list<std::unique_ptr<logger>> _logs;
      };
   }
}

#endif

#endif
