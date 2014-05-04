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

#include "shared_library.hh"

namespace hpc {

   shared_library::shared_library()
      : _hnd( 0 )
   {
   }

   shared_library::shared_library( fs::path const& path )
      : _hnd( 0 )
   {
      open( path );
   }

   shared_library::~shared_library()
   {
      close();
   }

   void
   shared_library::close()
   {
      if( _hnd )
      {
         INSIST( dlclose( _hnd ), == 0 );
         _hnd = 0;
      }
   }

   void
   shared_library::open( fs::path const& path,
			 int flags )
   {
      close();
      _hnd = dlopen( path.c_str(), flags );
      EXCEPT( _hnd, "Failed to load shared library: ", path, ": ", dlerror() );
   }

}
