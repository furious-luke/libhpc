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
