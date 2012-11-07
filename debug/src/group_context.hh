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

#ifndef libhpc_debug_group_context_hh
#define libhpc_debug_group_context_hh

#include <vector>
#include "assert.hh"
#include "group.hh"
#include "omp_lock.hh"
#include "checks.hh"

namespace hpc {
   namespace debug {

      template< class T >
      class group_context
      {
      public:

         static const int max_groups = 1024;

         group_context( bool enable_root=true )
	    : _num_groups( 0 )
	 {
	    select( "/" );
	 }

         void
         select( const char* path )
	 {
	    // Find an existing group, creating a new one if needed.
	    int grp = _find_create( path );

	    // Add the group to the current selection.
	    _groups[grp]._select_idx = _get_num_selected();
	    ++_groups[grp]._get_select_cnt();
	    _get_selection()[_get_num_selected()++] = grp;
	 }

         void
         deselect( const char* path )
	 {
	    int grp, *prev;
	    _find( path, grp, prev );
	    ASSERT( grp != -1, "Unable to locate a group for deselection." );
	    ASSERT( _groups[grp]._get_select_cnt() > 0, "Trying to deselect a group that is not already selected." );
	    if( --_groups[grp]._get_select_cnt() == 0 )
	    {
	       int sel = _groups[grp]._select_idx;
	       _groups[grp]._select_idx = -1;
	       _get_selection()[sel] = _get_selection()[_get_num_selected()--];
	       _groups[_get_selection()[sel]]._select_idx = sel;
	    }
	 }

         int
         num_groups() const
	 {
	    return _num_groups;
	 }

         const debug::group<T>&
         group( int idx ) const
	 {
	    ASSERT(idx >= 0 && idx < _num_groups, "Invalid group index.");
	    return _groups[idx];
	 }

         int
         num_selected()
	 {
	    return _get_num_selected();
	 }

         debug::group<T>&
         selected_group( int idx )
	 {
	    ASSERT( idx >= 0 && idx < _get_num_selected(), "Invalid selection index." );
	    return _groups[_get_selection()[idx]];
	 }

      protected:

         void
         _find( const char* path,
		int& group,
		int*& prev )
	 {
	    CHECK( check_path( path ) );

	    OMP_SET_LOCK( _find_lock );

	    group = -1;
	    prev = NULL;
	    int cmp;
	    if(_num_groups) {
	       group = 0;
	       do {
		  cmp = _groups[group]._cmp( path );
		  if(cmp < 0) {
		     group = _groups[group]._left;
		     prev = &_groups[group]._left;
		  }
		  else {
		     group = _groups[group]._right;
		     prev = &_groups[group]._right;
		  }
	       }
	       while(cmp != 0 && group != -1);
	    }

	    OMP_UNSET_LOCK( _find_lock );
	 }

         int
         _find_create( const char* path )
	 {
	    int grp, *prev;
	    _find( path, grp, prev );

	    OMP_SET_LOCK( _find_lock );

	    // Do we need to create a new one?
	    if( grp == -1 )
	    {
	       ASSERT( _num_groups < max_groups - 1,  "No more space left for groups." );
	       grp = _num_groups;
	       if( prev )
		  *prev = grp;
	       _groups[grp].set_path( path );
	       ++_num_groups;
	    }

	    OMP_UNSET_LOCK( _find_lock );

	    return grp;
	 }

	 int&
	 _get_num_selected()
	 {
	    OMP_SET_LOCK( _num_selected_lock );
	    auto res = _num_selected.insert( std::make_pair( OMP_TID, 0 ) );
	    OMP_UNSET_LOCK( _num_selected_lock );
	    return res.first->second;
	 }

	 std::vector<int>&
	 _get_selection()
	 {
	    OMP_SET_LOCK( _selection_lock );
	    std::vector<int>& sel = _selection[OMP_TID];
	    if( sel.size() == 0 )
	       sel.resize( max_groups );
	    OMP_UNSET_LOCK( _selection_lock );
	    return sel;
	 }

      private:

         int _num_groups;
         debug::group<T> _groups[max_groups];
	 std::map<int,int> _num_selected;
	 std::map<int,std::vector<int>> _selection;
	 OMP_LOCK( _find_lock );
	 OMP_LOCK( _num_selected_lock );
	 OMP_LOCK( _selection_lock );
      };
   }
}

#endif
