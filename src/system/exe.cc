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

#include <vector>
#ifdef DARWIN
#include <mach-o/dyld.h>
#endif
#include "libhpc/debug/insist.hh"
#include "exe.hh"

namespace hpc {
   namespace nix {

      fs::path
      executable_path()
      {
         // Get the path to (and including) the executable.
         int path_size = 100;
         std::vector<char> path( path_size + 1 );
         do
         {
            if( path_size >= path.size() )
               path.resize( 2*path_size );
            path_size = readlink( "/proc/self/exe", path.data(), path.size() );
         }
         while( path_size >= path.size() );

         // Return a boost filesystem path.
         return fs::path( path.begin(), path.end() );
      }

   }

#ifdef DARWIN

   namespace mac {

      fs::path
      executable_path()
      {
         // Get the path.
         char path[1024];
         uint32_t size = sizeof(path);
         INSIST( _NSGetExecutablePath( path, &size ), == 0 );

         // Return a boost filesystem path.
         return fs::path( path ); //.parent_path();
      }

   }

#endif
}
