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

#include <stdio.h>
#include "tmpfile.hh"

namespace hpc {

   tmpfile::tmpfile()
   {
      _gen_path();
   }

   tmpfile::tmpfile( std::ios_base::openmode mode )
   {
      _gen_path();
   }

   tmpfile::~tmpfile()
   {
      close();
   }

   void
   tmpfile::close()
   {
      if( !_path.empty() && fs::exists( _path ) )
         fs::remove( _path );
   }

   fs::path const&
   tmpfile::filename() const
   {
      return _path;
   }

   fs::path const&
   tmpfile::_gen_path()
   {
      char tmp[L_tmpnam + 1];
      tmpnam( tmp );
      _path = tmp;
      return _path;
   }

}
