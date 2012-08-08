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

#include "globals.hh"
#include "tracer.hh"

#ifndef NSTACKTRACE

namespace debug {

  tracer stack_trace;
  bool use_abort = false;

  void _clear_stack_trace() {
    stack_trace.clear();
  }

  void _enter_func(const char* func) {
    stack_trace.push(func);
  }

  void _exit_func() {
    stack_trace.pop();
  }

  void _set_abort(bool flag) {
    use_abort = flag;
  }
}

#endif
