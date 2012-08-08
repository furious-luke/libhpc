#ifndef debug_catch_hh
#define debug_catch_hh

#include "globals.hh"

#ifndef NDEBUG

#define CATCH(stmnt)				\
    try { stmnt; }				\
    catch(debug::assertion a) {			\
	std::cout << a.what() << std::endl;	\
    }

#define CATCH_ABORT(stmnt)			\
    do {					\
	set_abort(true);			\
	stmnt;					\
	set_abort(false);			\
    } while(0);

#else

#define CATCH(stmnt) stmnt
#define CATCH_ABORT(stmnt) stmnt

#endif

#endif
