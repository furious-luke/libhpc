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

#ifndef libhpc_debug_stacktrace_hh
#define libhpc_debug_stacktrace_hh

#ifndef NSTACKTRACE

#include <vector>
#include <string>

namespace hpc {
   namespace debug {

      class stacktrace
      {
      public:

         struct value_type
         {
            const char* file_name;
            char* func_name;
         };

         typedef const value_type* const_iterator;

         static const unsigned MAX_DEPTH;

      public:

         stacktrace();

         const_iterator
         begin() const;

         const_iterator
         end() const;

      protected:

         void
         _get_stack();

      protected:

         int _depth;
         value_type _st[32];
      };

   }
}

#endif

#endif
