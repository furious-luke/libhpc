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

#include <boost/range/adaptor/reversed.hpp>
#include "path_finder.hh"

namespace hpc {

   void
   path_finder::add_root( fs::path const& root )
   {
      _roots.emplace_back( root );
   }

   boost::optional<fs::path>
   path_finder::find( fs::path const& path )
   {
      // If path is absolute handle it separately.
      if( path.is_absolute() )
      {
	 if( fs::exists( path ) )
	    return path;
	 else
	    return boost::none;
      }

      // Search for compositions.
      for( auto const& root : boost::adaptors::reverse( _roots ) )
      {
	 fs::path tmp = root/path;
	 if( fs::exists( tmp ) )
	    return tmp;
      }
      return boost::none;
   }

   std::list<fs::path> const&
   path_finder::roots() const
   {
      return _roots;
   }

}
