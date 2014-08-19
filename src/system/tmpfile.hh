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

#ifndef libhpc_system_tmpfile_hh
#define libhpc_system_tmpfile_hh

#include <boost/filesystem.hpp>
#include <boost/move/move.hpp>

namespace hpc {
   namespace fs = boost::filesystem;

   class tmpfile
   {
      BOOST_MOVABLE_BUT_NOT_COPYABLE( tmpfile );

   public:

      tmpfile();

      tmpfile( std::ios_base::openmode mode );

      inline
      tmpfile( BOOST_RV_REF( tmpfile ) src )
         : _path( src._path )
      {
         src._path.clear();
      }

      ~tmpfile();

      inline
      tmpfile&
      operator=( BOOST_RV_REF( tmpfile ) src )
      {
         close();
         _path = src._path;
         src._path.clear();
         return *this;
      }

      void
      close();

      fs::path const&
      filename() const;

   protected:

      fs::path const&
      _gen_path();

   protected:

      fs::path _path;
   };

}

#endif
