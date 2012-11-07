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

#ifndef libhpc_memory_group_hh
#define libhpc_memory_group_hh

#include <string>
#include <map>
#include "libhpc/debug/debug.hh"
#include "stat_alloc.hh"

namespace hpc {
   namespace memory {

      template< class T >
      class group
      {
      public:

	 typedef std::basic_string<char, std::char_traits<char>, stat_alloc<char>> string;

      public:

         static const int max_path_length = 100;

	 group( const string& path )
	 {
	    set_path( path );
	 }

         ~group()
	 {
	 }

         void
         set_path( const string& path )
	 {
	    CHECK( debug::check_path( path.c_str() ) );
	    _path = path;
	 }

         const string&
         path() const
	 {
	    return _path;
	 }

         const T&
         data() const
	 {
	    return _data;
	 }

         T&
         data()
	 {
	    return _data;
	 }

	 bool
	 operator<( const group& op ) const
	 {
	    return _path < op._path;
	 }

      private:

         T _data;
	 string _path;
      };
   }
}

#endif
