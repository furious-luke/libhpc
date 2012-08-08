#include <string.h>
#include "checks.hh"
#include "group.hh"
#include "assert.hh"

#ifndef NDEBUG

namespace debug {

  void check_path(const char* path) {
    ASSERT(path, "Cannot use a null path.");
    ASSERT(strlen(path) < group<int>::max_path_length, "Path too long.");
    ASSERT(path[0] == '/', "Paths must begin with a forward slash.");
    const char* c = path;
    while(*c != 0) {
      ASSERT(
	     (*c >= 'a' && *c <= 'a') ||
	     (*c >= '0' && *c <= '9') ||
	     *c == '_' ||
	     *c == '/',
	     "Invalid path, must contain only lowercase alphanumeric, underbar and forward slash.");
      ++c;
    }
  }

}

#endif
