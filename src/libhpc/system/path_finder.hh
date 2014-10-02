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

#ifndef libhpc_system_path_finder_hh
#define libhpc_system_path_finder_hh

#include <boost/optional.hpp>
#include <boost/filesystem.hpp>

namespace hpc {
   namespace fs = boost::filesystem;

   class path_finder
   {
   public:

      path_finder();

      template< class Seq >
      path_finder( Seq const& roots )
         : _roots( roots.begin(), roots.end() )
      {
      }

      void
      add_root( fs::path const& root );

      boost::optional<fs::path>
      find( fs::path const& path );

      std::list<fs::path> const&
      roots() const;

   protected:

      std::list<fs::path> _roots;
   };

}

#endif
