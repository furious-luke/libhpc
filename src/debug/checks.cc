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

#include <string.h>
#include "checks.hh"
#include "group.hh"
#include "assert.hh"

#ifndef NDEBUG

namespace hpc {
   namespace debug {

      void
      check_path( const char* path )
      {
         ASSERT( path, "Cannot use a null path." );
         ASSERT( strlen( path ) < group<int>::max_path_length, "Path too long." );
         ASSERT( path[0] == '/', "Paths must begin with a forward slash." );
         const char* c = path;
         while( *c != 0 )
         {
            ASSERT(
               (*c >= 'a' && *c <= 'z') ||
               (*c >= 'A' && *c <= 'Z') ||
               (*c >= '0' && *c <= '9') ||
               *c == '_' ||
               *c == '/',
               "Invalid path, must contain only lowercase alphanumeric, underbar and forward slash.");
            ++c;
         }
      }
   }
}

#endif
