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

#ifndef libhpc_logging_block_hh
#define libhpc_logging_block_hh

#include "libhpc/system/anon.hh"
#include "libhpc/system/stream_indent.hh"
#include "globals.hh"

#ifndef NLOG

#define LOGBLOCK( ... )                         \
   LOGLN( __VA_ARGS__, ::hpc::setindent( 1 ) ); \
   ::hpc::logging::block ANON

#ifndef NLOGTRIVIAL

#define LOGBLOCKT( ... )                                        \
   LOGTLN( __VA_ARGS__, ::hpc::setindent( 1 ) );                \
   ::hpc::logging::block ANON( ::hpc::logging::trivial )

#else

#define LOGBLOCKT( ... )

#endif

#ifndef NLOGDEBUG

#define LOGBLOCKD( ... )                                \
   LOGDLN( __VA_ARGS__, ::hpc::setindent( 1 ) );        \
   ::hpc::logging::block ANON( ::hpc::logging::debug )

#else

#define LOGBLOCKD( ... )

#endif

namespace hpc {
   namespace logging {

      class block
      {
      public:

         block( levels_type lvl = (levels_type)0 );

         ~block();

      protected:

         unsigned _lvl;
      };

   }
}

#else

#define LOGBLOCK( ... )

#endif

#endif
