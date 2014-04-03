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

#ifndef NLOG

#include "libhpc/system/anon.hh"
#include "libhpc/system/stream.hh"
#include "globals.hh"

#define LOGBLOCK( ... )                         \
   LOGLN( __VA_ARGS__, ::hpc::setindent( 1 ) ); \
   ::hpc::log::block ANON

#define LOGBLOCK_TAG( tag, ... )		\
   LOG_PUSH_TAG( tag );                         \
   LOGLN( __VA_ARGS__, ::hpc::setindent( 1 ) ); \
   ::hpc::log::block ANON( tag )

#ifndef NLOGTRIVIAL

#define LOGBLOCKT( ... )                                \
   LOGTLN( __VA_ARGS__, ::hpc::setindent( 1 ) );        \
   ::hpc::log::block ANON( ::hpc::log::trivial )

#define LOGBLOCKT_TAG( tag, ... )                       \
   LOG_PUSH_TAG( tag );                                 \
   LOGTLN( __VA_ARGS__, ::hpc::setindent( 1 ) );        \
   ::hpc::log::block ANON( tag, ::hpc::log::trivial )

#else

#define LOGBLOCKT( ... )
#define LOGBLOCKT_TAG( ... )

#endif

#ifndef NLOGDEBUG

#define LOGBLOCKD( ... )                                \
   LOGDLN( __VA_ARGS__, ::hpc::setindent( 1 ) );        \
   ::hpc::log::block ANON( ::hpc::log::debug )

#define LOGBLOCKD_TAG( tag, ... )                       \
   LOG_PUSH_TAG( tag );                                 \
   LOGDLN( __VA_ARGS__, ::hpc::setindent( 1 ) );        \
   ::hpc::log::block ANON( tag, ::hpc::log::debug )

#else

#define LOGBLOCKD( ... )
#define LOGBLOCKD_TAG( ... )

#endif

#define LOGBLOCKI( ... )                                \
   LOGILN( __VA_ARGS__, ::hpc::setindent( 1 ) );        \
   ::hpc::log::block ANON( ::hpc::log::info )

#define LOGBLOCKI_TAG( tag, ... )                       \
   LOG_PUSH_TAG( tag );                                 \
   LOGILN( __VA_ARGS__, ::hpc::setindent( 1 ) );        \
   ::hpc::log::block ANON( tag, ::hpc::log::info )

   namespace hpc {
namespace log {

   class block
   {
   public:

      block( levels_type lvl = (levels_type)0 );

      block( std::string const& tag,
             levels_type lvl = (levels_type)0 );

      ~block();

   protected:

      unsigned _lvl;
      std::string _tag;
   };

}
   }

#else

#define LOGBLOCK( ... )
#define LOGBLOCK_TAG( ... )
#define LOGBLOCKI( ... )
#define LOGBLOCKI_TAG( ... )
#define LOGBLOCKD( ... )
#define LOGBLOCKD_TAG( ... )
#define LOGBLOCKT( ... )
#define LOGBLOCKT_TAG( ... )

#endif

#endif
