#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "string.hh"

namespace hpc {

   void
   to_lower( std::string& str )
   {
      std::transform( str.begin(), str.end(), str.begin(), ::tolower );
   }

   std::string
   to_lower( std::string const& str )
   {
      return boost::algorithm::to_lower_copy( str );
   }

}
