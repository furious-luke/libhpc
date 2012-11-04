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

#ifndef libhpc_memory_group_context_hh
#define libhpc_memory_group_context_hh

#include <map>
#include <set>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/utility/enable_if.hpp>
#include "libhpc/debug/debug.hh"
#include "group.hh"
#include "stat_alloc.hh"

namespace hpc {
   namespace memory {

      template< class T >
      class group_iterator;

      template< class T >
      class group_context
      {
      public:

	 typedef std::basic_string<char,std::char_traits<char>,stat_alloc<char>> string_type;
	 typedef std::map<group<T>*,int,std::less<group<T>*>,stat_alloc<std::pair<group<T>*,int>>> group_map;
	 typedef std::map<int,group_map,std::less<int>,stat_alloc<std::pair<int,group_map>>> select_map;
	 typedef std::set<group<T>,std::less<group<T>>,stat_alloc<group<T>>> group_set;

	 typedef group_iterator<T> iterator;

      public:

         group_context( bool enable_root=true )
	 {
	    if( enable_root )
	       select( string_type( "/" ) );
	 }

         void
         select( const string_type& path )
	 {
	    CHECK( debug::check_path( path.c_str() ) );

	    OMP_SET_LOCK( _group_lock );
	    auto grp_res = _groups.insert( group<T>( path ) );
	    OMP_UNSET_LOCK( _group_lock );

	    OMP_SET_LOCK( _selection_lock );
	    auto& the_map = _get_selection();
	    auto sel_res = _get_selection().insert( std::make_pair( (group<T>*)&(*grp_res.first), 0 ) );
	    OMP_UNSET_LOCK( _selection_lock );

	    ++sel_res.first->second;
	 }

         void
         deselect( const string_type& path )
	 {
	    CHECK( debug::check_path( path.c_str() ) );

	    OMP_SET_LOCK( _group_lock );
	    auto grp_it = _groups.find( group<T>( path ) );
	    ASSERT( grp_it != _groups.end(),
		    "No group matching specified path." );
	    OMP_UNSET_LOCK( _group_lock );

	    OMP_SET_LOCK( _selection_lock );
	    auto sel_it = _get_selection().find( (group<T>*)&(*grp_it) );
	    ASSERT( sel_it != _get_selection().end(),
		    "Unable to locate a group for deselection." );
	    ASSERT( sel_it->second > 0,
		    "Trying to deselect a group that is not already selected." );
	    if( --sel_it->second == 0 )
	       _get_selection().erase( sel_it );
	    OMP_UNSET_LOCK( _selection_lock );
	 }

	 iterator
	 begin()
	 {
	    return iterator( _get_selection().begin() );
	 }

	 iterator
	 end()
	 {
	    return iterator( _get_selection().end() );
	 }

	 const group<T>&
	 find_group( const string_type& path )
	 {
	    OMP_SET_LOCK( _group_lock );
	    auto it = _groups.find( group<T>( path ) );
	    OMP_UNSET_LOCK( _group_lock );
	    ASSERT( it != _groups.end() );
	    return *it;
	 }

      protected:

	 group_map&
	 _get_selection()
	 {
	    OMP_SET_LOCK( _selection_lock );
	    auto& sel = _selection[OMP_TID];
	    OMP_UNSET_LOCK( _selection_lock );
	    return sel;
	 }

      private:

	 group_set _groups;
	 select_map _selection;
	 OMP_LOCK( _group_lock );
	 OMP_NEST_LOCK( _selection_lock );
      };

      template< class T >
      class group_iterator
	 : public boost::iterator_adaptor< group_iterator<T>,
					   typename group_context<T>::group_map::const_iterator,
					   group<T>&,
					   boost::forward_traversal_tag,
					   group<T>& >
      {
	 friend class boost::iterator_core_access;

	 struct enabler {};
	 
	 typedef typename group_context<T>::group_map::const_iterator super_it_type;

      public:

	 explicit group_iterator( super_it_type it )
	    : group_iterator::iterator_adaptor_( it )
	 {
	 }

	 template< class U >
	 group_iterator( group_iterator<U>& other,
			 typename boost::enable_if<boost::is_convertible<U,super_it_type>,enabler>::type=enabler() )
	    : group_iterator::iterator_adaptor_( other.base() )
	 {
	 }

      protected:

	 group<T>&
	 dereference() const
	 {
	    return *this->base()->first;
	 }
      };
   }
}

#endif
