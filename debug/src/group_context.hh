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

#ifndef debug_group_context_hh
#define debug_group_context_hh

#include "group.hh"

namespace debug {

   template< class T >
   class group_context
   {
   public:

      static const int max_groups = 1024;

      group_context( bool enable_root=true );

      ~group_context();

      void
      select( const char* path );

      void
      deselect( const char* path );

      int
      num_groups() const;

      const debug::group<T>&
      group( int idx ) const;

      int
      num_selected() const;

      debug::group<T>&
      selected_group( int idx );

   protected:

      void
      find( const char* path,
	    int& group,
	    int*& prev );

      int
      find_create( const char* path );

   private:

      int _num_groups;
      debug::group<T> _groups[max_groups];
      int _num_selected;
      int _selection[max_groups];
   };
}

#endif
