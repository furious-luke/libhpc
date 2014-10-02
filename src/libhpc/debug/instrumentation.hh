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

#ifndef hpc_debug_instrumentation_hh
#define hpc_debug_instrumentation_hh

#include "config.hh"

#ifndef NINSTRUMENT

namespace hpc {
   namespace debug {

      ///
      /// Interface for instrumentation.
      ///
      class instrument
      {
      public:

         instrument();

         virtual
         void
         enter( const char* file_name,
                const char* func_name ) = 0;

         virtual
         void
         exit( const char* file_name,
               const char* func_name ) = 0;
      };
   }
}

#endif

#endif