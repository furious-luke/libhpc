#ifndef debug_globals_hh
#define debug_globals_hh

#ifndef NSTACKTRACE

#define CLEAR_STACK_TRACE()			\
  debug::_clear_stack_trace()

#define ENTER_FUNC()				\
  debug::_enter_func(__PRETTY_FUNCTION__)

#define EXIT_FUNC()				\
  debug::_exit_func()

#define SET_ABORT(f)				\
  debug::_set_abort(f)

namespace debug {

  void _clear_stack_trace();

  void _enter_func(const char* func);

  void _exit_func();

  void _set_abort(bool flag);
}

#else

#define CLEAR_STACK_TRACE()
#define ENTER_FUNC()
#define EXIT_FUNC()
#define SET_ABORT(f)

#endif

#endif
