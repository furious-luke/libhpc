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

#ifndef debug_assert_hh
#define debug_assert_hh

#ifndef NDEBUG

#include <cstdlib>
#include "assertions.hh"

#define ASSERT( expr, ... )                                     \
   ::hpc::debug::_assert( expr, __FILE__,                       \
                          __LINE__, #expr, ##__VA_ARGS__ )

namespace hpc {
   namespace debug {

      void
      _assert( bool state,
               const char* file,
               int line,
               const char* expr,
               const char* msg=NULL );

      void
      _assert( bool state,
               const char* file,
               int line,
               const char* expr,
               assertion ass );

      void
      _assert( bool state,
               const char* file,
               int line,
               const char* expr,
               int code );
   }
}

#else

#define ASSERT( expr, ... )

#endif

#endif
