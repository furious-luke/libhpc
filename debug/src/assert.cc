#include <stdlib.h>
#include <stdio.h>
#include "assert.hh"

#ifndef NDEBUG

namespace debug {

  extern bool use_abort;

  void _assert(bool state, const char* file, int line, const char* expr, const char* msg) {
    _assert(state, file, line, expr, assertion(msg));
  }

  void _assert(bool state, const char* file, int line, const char* expr, assertion ass) {
    if(!state) {
      ass.details(file, line, expr);
      if(use_abort)
	abort();
      else
	throw ass;
    }
  }

  void _assert(bool state, const char* file, int line, const char* expr, int code) {
    _assert(state, file, line, expr, assertion());
  }

}

#endif
