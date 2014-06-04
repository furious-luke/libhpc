// Copyright 2012 Luke Hodkinson

// This file is part of libhpc.
// 
// libhpc is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// libhpc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with libhpc.  If not, see <http://www.gnu.org/licenses/>.

#ifndef libhpc_system_string_hh
#define libhpc_system_string_hh

#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace hpc {

   template< class T >
   std::string
   index_string( T idx,
		 unsigned width = 5 )
   {
      std::stringstream ss;
      ss << std::setfill( '0' ) << std::setw( width ) << idx;
      return ss.str();
   }

   inline
   void
   to_lower( std::string& str )
   {
      std::transform( str.begin(), str.end(), str.begin(), ::tolower );
   }

   inline
   std::string
   to_lower_copy( std::string const& str )
   {
      return boost::algorithm::to_lower_copy( str );
   }

   template< class T >
   inline
   std::string
   to_string( T val )
   {
      return boost::lexical_cast<std::string>( val );
   }

}

#endif
