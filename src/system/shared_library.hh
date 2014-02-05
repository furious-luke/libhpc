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

#ifndef libhpc_system_shared_library_hh
#define libhpc_system_shared_library_hh

#include <string>
#include <boost/filesystem.hpp>
#include <dlfcn.h>
#include "libhpc/debug/debug.hh"

namespace hpc {
   namespace fs = boost::filesystem;

   class shared_library
   {
   public:

      shared_library();

      shared_library( fs::path const& path );

      shared_library( shared_library&& src );

      ~shared_library();

      void
      close();

      void
      open( fs::path const& path,
	    int flags = RTLD_LAZY | RTLD_GLOBAL );

      template< class T >
      T*
      symbol( std::string const& name )
      {
	 T* sym = (T*)dlsym( _hnd, name.c_str() );
         EXCEPT( sym, "Failed to load symbol from shared library: ", name );
         return sym;
      }

      shared_library&
      operator=( shared_library&& src );

   protected:

      void* _hnd;
   };

}

#endif
