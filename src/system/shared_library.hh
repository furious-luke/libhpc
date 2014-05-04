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
#include <boost/move/move.hpp>
#include <dlfcn.h>
#include "libhpc/debug.hh"
#include "filesystem.hh"

namespace hpc {

   class shared_library
   {
      BOOST_MOVABLE_BUT_NOT_COPYABLE( shared_library );

   public:

      shared_library();

      shared_library( fs::path const& path );

      inline
      shared_library( BOOST_RV_REF( shared_library ) src )
         : _hnd( src._hnd )
      {
         src._hnd = 0;
      }

      ~shared_library();

      inline
      shared_library&
      operator=( BOOST_RV_REF( shared_library ) src )
      {
         _hnd = src._hnd;
         src._hnd = 0;
         return *this;
      }

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

   protected:

      void* _hnd;
   };

}

#endif
