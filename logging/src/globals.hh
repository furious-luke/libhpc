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

#include "logger.hh"
#include "stack.hh"

#ifndef NLOG

#define PP_NARG( ... )                          \
   PP_NARG_( __VA_ARGS__, PP_RSEQ_N() )
#define PP_NARG_(...)                           \
   PP_ARG_N(__VA_ARGS__) 
#define PP_ARG_N(                               \
   _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,      \
   _11,_12,_13,_14,_15,_16,_17,_18,_19,_20,     \
   _21,_22,_23,_24,_25,_26,_27,_28,_29,_30,     \
   _31,_32,_33,_34,_35,_36,_37,_38,_39,_40,     \
   _41,_42,_43,_44,_45,_46,_47,_48,_49,_50,     \
   _51,_52,_53,_54,_55,_56,_57,_58,_59,_60,     \
   _61,_62,_63,N,...) N
#define PP_RSEQ_N()                             \
   63,62,61,60,                                 \
      59,58,57,56,55,54,53,52,51,50,            \
      49,48,47,46,45,44,43,42,41,40,            \
      39,38,37,36,35,34,33,32,31,30,            \
      29,28,27,26,25,24,23,22,21,20,            \
      19,18,17,16,15,14,13,12,11,10,            \
      9,8,7,6,5,4,3,2,1,0

#define _LOG1( strm, p0 ) (strm << p0)
#define _LOG2( strm, p0, p1 ) (strm << p0 << p1)
#define _LOG3( strm, p0, p1, p2 ) (strm << p0 << p1 << p2)
#define _LOG4( strm, p0, p1, p2, p3 ) (strm << p0 << p1 << p2 << p3)
#define _LOG5( strm, p0, p1, p2, p3, p4 ) (strm << p0 << p1 << p2 << p3 << p4)
#define _LOG6( strm, p0, p1, p2, p3, p4, p5 ) (strm << p0 << p1 << p2 << p3 << p4 << p5)
#define _LOG7( strm, p0, p1, p2, p3, p4, p5, p6 ) (strm << p0 << p1 << p2 << p3 << p4 << p5 << p6)

#define _LOG_( count, ... )                                     \
   _LOG ## count ( ::hpc::logging::_stack, __VA_ARGS__ )

#define _LOG( count, ... )                      \
   _LOG_( count, __VA_ARGS__ )

#define LOG( ... )                              \
   _LOG( PP_NARG( __VA_ARGS__ ), __VA_ARGS__ )

#define LOG_ENTER()                             \
   LOG( "Entering: ", __PRETTY_FUNCTION__, ::hpc::logging::endl, strm_setindent( 2 ) )

#define LOG_EXIT()                              \
   LOG( strm_setindent( -2 ), "Exiting: ", __PRETTY_FUNCTION__, ::hpc::logging::endl )

#define LOG_PUSH( logger )                      \
   ::hpc::logging::push( logger )

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
#define LOG_ENTER()
#define LOG_EXIT()
#define LOG_PUSH( logger )
#define LOG_POP()

#endif

#endif
