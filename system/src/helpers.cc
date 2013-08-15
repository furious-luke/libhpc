#include <algorithm>
#include "helpers.hh"

namespace hpc {

   void
   to_lower( string& str )
   {
      std::transform( str.begin(), str.end(), str.begin(), ::tolower );
   }

}
