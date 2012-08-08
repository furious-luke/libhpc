#include "types.hh"

namespace hpc {

   std::ostream&
   operator<<( std::ostream& strm,
               byte_t val )
   {
      strm << (int)val;
      return strm;
   }
}
