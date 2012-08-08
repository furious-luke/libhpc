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
