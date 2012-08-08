#ifndef libhpc_system_types_hh
#define libhpc_system_types_hh

#include <stdlib.h>
#include <ostream>

typedef unsigned char byte_t;

namespace hpc {

   typedef byte_t byte;
   typedef size_t index;

   std::ostream&
   operator<<( std::ostream& strm,
               byte_t val );
}

#endif
