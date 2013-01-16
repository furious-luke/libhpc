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

#ifndef libhpc_debug_error_hh
#define libhpc_debug_error_hh

#include <cstdlib>
#include "libhpc/system/narg.hh"

#define _ERRORS1( p0 ) p0 = __COUNTER__
#define _ERRORS2( p0, p1 ) _ERRORS1( p0 ), p1 = __COUNTER__
#define _ERRORS3( p0, p1, p2 ) _ERRORS2( p0, p1 ), p2 = __COUNTER__
#define _ERRORS4( p0, p1, p2, p3 ) _ERRORS3( p0, p1, p2 ), p3 = __COUNTER__

#define __ERRORS( count, ... )                  \
   _ERRORS##count( __VA_ARGS__ )

#define _ERRORS( count, ... )                   \
   __ERRORS( count, __VA_ARGS__ )

#define ERRORS( ... )                                   \
   enum error {                                         \
      _ERRORS( PP_NARG( __VA_ARGS__ ), __VA_ARGS__ )    \
   }

#define SETERR( code, place )                   \
   ::hpc::debug::error_code = code;             \
   goto place

#define HNDERR( cond, code, place )             \
   if( cond )                                   \
   {                                            \
      SETERR( code, place );                    \
   }                                            \
   while( 0 )

#define ERROK()                                         \
   ::hpc::debug::error_code = ::hpc::debug::error::okay

#define CHKERR()                                                \
   if( ::hpc::debug::error_code != ::hpc::debug::error::okay )  \
   {                                                            \
      LOGELN( "Error" );                                        \
      abort();                                                  \
   }                                                            \
   while( 0 )

namespace hpc {
   namespace debug {

      extern __thread unsigned long error_code;
   }
}

#endif
