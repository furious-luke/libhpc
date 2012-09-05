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

#ifndef debug_globals_hh
#define debug_globals_hh

#ifndef NSTACKTRACE

#define CLEAR_STACK_TRACE()			\
   ::hpc::debug::_clear_stack_trace()

// TODO: Remove.
#define ENTER_FUNC()                                    \
   ::hpc::debug::_enter_func(__PRETTY_FUNCTION__)

// TODO Remove
#define EXIT_FUNC()				\
   ::hpc::debug::_exit_func()

#define TRACE_ENTER()                                   \
   ::hpc::debug::_enter_func(__PRETTY_FUNCTION__)

#define TRACE_EXIT()                            \
   ::hpc::debug::_exit_func()

#define SET_ABORT(f)				\
   ::hpc::debug::_set_abort(f)

namespace hpc {
   namespace debug {

      void _clear_stack_trace();

      void _enter_func(const char* func);

      void _exit_func();

      void _set_abort(bool flag);
   }
}

#else

#define CLEAR_STACK_TRACE()
#define ENTER_FUNC()
#define EXIT_FUNC()
#define TRACE_ENTER()
#define TRACE_EXIT()
#define SET_ABORT(f)

#endif

#endif
