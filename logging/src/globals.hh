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

#ifndef libhpc_logging_globals_hh
#define libhpc_logging_globals_hh

#include "libhpc/system/stream_indent.hh"
#include "libhpc/system/narg.hh"
#include "logger.hh"
#include "stack.hh"

#ifndef NLOG

#define _LOG1( strm, p0 ) (strm << p0)
#define _LOG2( strm, p0, p1 ) (strm << p0 << p1)
#define _LOG3( strm, p0, p1, p2 ) (strm << p0 << p1 << p2)
#define _LOG4( strm, p0, p1, p2, p3 ) (strm << p0 << p1 << p2 << p3)
#define _LOG5( strm, p0, p1, p2, p3, p4 ) (strm << p0 << p1 << p2 << p3 << p4)
#define _LOG6( strm, p0, p1, p2, p3, p4, p5 ) (strm << p0 << p1 << p2 << p3 << p4 << p5)
#define _LOG7( strm, p0, p1, p2, p3, p4, p5, p6 ) (strm << p0 << p1 << p2 << p3 << p4 << p5 << p6)
#define _LOG8( strm, p0, p1, p2, p3, p4, p5, p6, p7 ) (strm << p0 << p1 << p2 << p3 << p4 << p5 << p6 << p7)
#define _LOG9( strm, p0, p1, p2, p3, p4, p5, p6, p7, p8 ) (strm << p0 << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8)

#define _LOG_( count, ... )                             \
   _LOG##count( ::hpc::logging::_stack, __VA_ARGS__ )

#define _LOG( count, ... )                      \
   _LOG_( count, __VA_ARGS__ )

#define LOG( ... )                              \
   _LOG( PP_NARG( __VA_ARGS__ ), __VA_ARGS__ )

#define LOGLN( ... )                            \
   LOG( __VA_ARGS__, ::hpc::logging::endl )

#define LOGLV( level, ... )                                     \
   LOG( ::hpc::logging::pushlevel( level ), __VA_ARGS__,        \
        ::hpc::logging::poplevel )

#define LOGLVLN( level, ... )                                   \
   LOG( ::hpc::logging::pushlevel( level ), __VA_ARGS__,        \
        ::hpc::logging::endl, ::hpc::logging::poplevel )

#define LOGT( ... )                             \
   LOGLV( ::hpc::logging::debug, __VA_ARGS__ )

#define LOGD( ... )                             \
   LOGLV( ::hpc::logging::debug, __VA_ARGS__ )

#define LOGI( ... )                             \
   LOGLV( ::hpc::logging::debug, __VA_ARGS__ )

#define LOGLNT( ... )                                   \
   LOGLVLN( ::hpc::logging::debug, __VA_ARGS__ )

#define LOGLND( ... )                                   \
   LOGLVLN( ::hpc::logging::debug, __VA_ARGS__ )

#define LOGLNI( ... )                                   \
   LOGLVLN( ::hpc::logging::debug, __VA_ARGS__ )

#define LOG_ENTER()                                             \
   TRACE_ENTER();                                               \
   LOG( ::hpc::logging::pushlevel( 0 ), "Entering: ",           \
        __PRETTY_FUNCTION__, ::hpc::logging::endl,              \
        ::hpc::setindent( 2 ), ::hpc::logging::poplevel )

#define LOG_EXIT()                                              \
   LOG( ::hpc::logging::pushlevel( 0 ), ::hpc::setindent( -2 ), \
        "Exiting: ", __PRETTY_FUNCTION__, ::hpc::logging::endl, \
        ::hpc::logging::poplevel );                             \
   TRACE_EXIT()

#define LOG_PUSH( logger )                      \
   ::hpc::logging::push( logger )

#define LOG_FILE( filename )                    \
   ::hpc::logging::push(                        \
      new ::hpc::logging::file( filename ) )

#define LOG_CONSOLE()                           \
   ::hpc::logging::push(                        \
      new ::hpc::logging::stdout() )

#define LOG_POP()                               \
   ::hpc::logging::pop()

namespace hpc {
   namespace logging {

      extern stack _stack;

      void
      push( logger* log );

      void
      pop();

      void
      clear();
   }
}

#else

#define LOG( ... )
#define LOGLN( ... )
#define LOGLV( level, ... )
#define LOGLVLN( level, ... )
#define LOGT( ... )
#define LOGD( ... )
#define LOGI( ... )
#define LOGTLN( ... )
#define LOGDLN( ... )
#define LOGILN( ... )
#define LOG_ENTER()
#define LOG_EXIT()
#define LOG_PUSH( logger )
#define LOG_FILE( filename )
#define LOG_CONSOLE()
#define LOG_POP()

#endif

#endif
