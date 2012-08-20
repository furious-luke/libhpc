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

#include "stack.hh"

#ifndef NLOG

namespace hpc {
   namespace logging {

      stack::stack()
      {
      }

      stack::~stack()
      {
      }

      void
      stack::push( logger* log )
      {
         log->open();
         _logs.push_back( log );
      }

      void
      stack::pop()
      {
         _logs.back()->close();
         _logs.pop_back();
      }

      void
      stack::clear()
      {
         for( auto& log : _logs )
            log->close();
         _logs.clear();
      }
   }
}

#endif
