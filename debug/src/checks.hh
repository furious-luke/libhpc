#ifndef debug_checks_hh
#define debug_checks_hh

#ifndef NDEBUG

#define CHECK(expr) expr

namespace debug {

  void check_path(const char* path);

}

#else

#define CHECK(expr)

#endif

#endif
