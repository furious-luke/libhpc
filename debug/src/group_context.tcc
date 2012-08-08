#ifndef debug_group_context_tcc
#define debug_group_context_tcc

#include "checks.hh"

namespace debug {

   template< class T >
   const int group_context<T>::max_groups;

   template< class T >
   group_context<T>::group_context( bool enable_root )
      : _num_groups(0),
	_num_selected(0)
   {
      this->select("/");
   }

   template< class T >
   group_context<T>::~group_context()
   {
   }

   template< class T >
   void
   group_context<T>::select( const char* path )
   {
      // Find an existing group, creating a new one if needed.
      int grp = this->find_create(path);

      // Add the group to the current selection.
      this->_groups[grp]._select_idx = this->_num_selected;
      ++this->_groups[grp]._select_cnt;
      this->_selection[this->_num_selected++] = grp;
   }

   template< class T >
   void
   group_context<T>::deselect( const char* path )
   {
      int grp, *prev;
      this->find(path, grp, prev);
      ASSERT(grp != -1, "Unable to locate a group for deselection.");
      ASSERT(this->_groups[grp]._select_cnt > 0, "Trying to deselect a group that is not already selected.");
      if(--this->_groups[grp]._select_cnt == 0) {
	 int sel = this->_groups[grp]._select_idx;
	 this->_groups[grp]._select_idx = -1;
	 this->_selection[sel] = this->_selection[this->_num_selected--];
	 this->_groups[this->_selection[sel]]._select_idx = sel;
      }
   }

   template< class T >
   int
   group_context<T>::num_groups() const
   {
      return this->_num_groups;
   }

   template< class T >
   const group<T>&
   group_context<T>::group( int idx ) const
   {
      ASSERT(idx >= 0 && idx < this->_num_groups, "Invalid group index.");
      return this->_groups[idx];
   }

   template< class T >
   int
   group_context<T>::num_selected() const
   {
      return this->_num_selected;
   }

   template< class T >
   group<T>&
   group_context<T>::selected_group( int sel_idx )
   {
      ASSERT(sel_idx >= 0 && sel_idx < this->_num_selected, "Invalid selection index.");
      return this->_groups[this->_selection[sel_idx]];
   }

   template< class T >
   void
   group_context<T>::find( const char* path,
			   int& group,
			   int*& prev )
   {
#ifndef NDEBUG
      check_path(path);
#endif

      group = -1;
      prev = NULL;
      int cmp;
      if(this->_num_groups) {
	 group = 0;
	 do {
	    cmp = this->_groups[group].compare(path);
	    if(cmp < 0) {
	       group = this->_groups[group]._left;
	       prev = &this->_groups[group]._left;
	    }
	    else {
	       group = this->_groups[group]._right;
	       prev = &this->_groups[group]._right;
	    }
	 }
	 while(cmp != 0 && group != -1);
      }
   }

   template< class T >
   int
   group_context<T>::find_create( const char* path )
   {
      int grp, *prev;
      this->find(path, grp, prev);

      // Do we need to create a new one?
      if(grp == -1) {
	 ASSERT(this->_num_groups < this->max_groups - 1,  "No more space left for groups.");
	 grp = this->_num_groups;
	 if(prev)
	    *prev = grp;
	 this->_groups[grp].set_path(path);
	 ++this->_num_groups;
      }
      return grp;
   }

}

#endif
