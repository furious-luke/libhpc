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

#ifndef libhpc_debug_group_hh
#define libhpc_debug_group_hh

#include <string.h>
#include <map>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "omp_lock.hh"
#include "omp_help.hh"
#include "checks.hh"

namespace hpc {
   namespace debug {

      template< class T >
      class group_context;

      template< class T >
      class group
      {
         friend class group_context<T>;

      public:

         static const int max_path_length = 100;

         group()
	    : _left( -1 ),
	      _right( -1 ),
	      _select_idx( -1 )
	 {
	 }

         ~group()
	 {
	 }

         void
         set_path( const char* path )
	 {
	    CHECK( check_path( path ) );
	    strcpy( _path, path );
	 }

         const char*
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

      protected:

         int
	 _cmp( const char* path ) const
	 {
	    return strcmp( _path, path );
	 }

	 int&
	 _get_select_cnt()
	 {
	    OMP_SET_LOCK( _lock );
	    int& cnt = _select_cnt[OMP_TID];
	    OMP_UNSET_LOCK( _lock );
	    return cnt;
	 }

      private:

         T _data;
         char _path[max_path_length + 1];
         int _left;
         int _right;
         int _select_idx;
	 std::map<int,int> _select_cnt;
	 OMP_LOCK( _lock );
      };
   }
}

#endif
