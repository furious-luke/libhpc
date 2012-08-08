#ifndef libhpc_system_stream_indent_hh
#define libhpc_system_stream_indent_hh

#include <iostream>
#include <fstream>
#include "types.hh"

namespace hpc {

   struct setindent_t {
      int indent;
   };

   setindent_t
   setindent( int indent );

   std::ostream&
   operator<<( std::ostream& strm,
               setindent_t si );

   std::ostream&
   indent( std::ostream& strm );
}

#endif
