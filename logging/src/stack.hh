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

#include "libhpc/memory/memory.hh"
#include "libhpc/containers/list.hh"
#include "libhpc/containers/scoped_ptr.hh"
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

         stack();

         ~stack();

         void
         push( logger* log );

         void
         pop();

         void
         clear();

         template< class T >
         stack&
         operator<<( const T& obj )
         {
            for( auto& log : _logs )
               (*log) << obj;
            return *this;
         }

      protected:

         list<scoped_ptr<logger>> _logs;
      };
   }
}

#endif

#endif
