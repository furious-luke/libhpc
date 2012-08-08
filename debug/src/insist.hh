#ifndef debug_insist_hh
#define debug_insist_hh

#include "assert.hh"

#ifndef NDEBUG

#define INSIST(stmnt, cond, ...) ASSERT(stmnt cond, ##__VA_ARGS__)

#else

#define INSIST(stmnt, cond, ...) stmnt

#endif

#endif
