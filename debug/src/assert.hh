#ifndef debug_assert_hh
#define debug_assert_hh

#ifndef NDEBUG

#include <stdlib.h>
#include "assertions.hh"

#define ASSERT(expr, ...)					\
  debug::_assert(expr, __FILE__, __LINE__, #expr, ##__VA_ARGS__)

namespace debug {

  void _assert(bool state, const char* file, int line, const char* expr, const char* msg=NULL);

  void _assert(bool state, const char* file, int line, const char* expr, assertion ass);

  void _assert(bool state, const char* file, int line, const char* expr, int code);

}

#else

#define ASSERT(expr, ...)

#endif

#endif
