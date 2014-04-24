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

#ifndef libhpc_system_assign_hh
#define libhpc_system_assign_hh

#include <vector>
#include <set>
#include "view.hh"
#include "deallocate.hh"

namespace hpc {

   template< class T,
             class U >
   T&
   assign( T& tgt,
           U&& src )
   {
      return tgt = std::forward<U>( src );
   }

   template< class T,
             class U >
   std::vector<T>&
   assign( std::vector<T>& tgt,
           view<std::vector<U>> const& src )
   {
      tgt.resize( src.size() );
      std::copy( src.begin(), src.end(), tgt.begin() );
      return tgt;
   }

   template< class T,
             class U >
   std::vector<T>&
   assign( std::vector<T>& tgt,
           view<std::vector<U>>&& src )
   {
      tgt.resize( src.size() );
      std::copy( src.begin(), src.end(), tgt.begin() );
      return tgt;
   }

   ///
   /// Adopt source view. When moving a source view to a target view
   /// I can copy the pointer and size.
   ///
   /// @param tgt Target view to adopt values.
   /// @param src Source view to provide values.
   /// @return Reference to target view.
   ///
   template< class T >
   view<std::vector<T>>&
   assign( view<std::vector<T>>& tgt,
           view<std::vector<T>>&& src )
   {
      tgt.assign( src );
      return tgt;
   }

   ///
   /// Adopt source vector.
   ///
   /// @param tgt Target view to adopt values.
   /// @param src Source vector to provide values.
   /// @return Reference to target view.
   ///
   template< class T >
   view<std::vector<T>>&
   assign( view<std::vector<T>>& tgt,
           std::vector<T> const& src )
   {
      tgt.assign( src );
      return tgt;
   }

   ///
   /// Adopt source vector.
   ///
   /// @param tgt Target view to adopt values.
   /// @param src Source vector to provide values.
   /// @return Reference to target view.
   ///
   template< class T >
   view<std::vector<T>>&
   assign( view<std::vector<T>>& tgt,
           std::vector<T>& src )
   {
      tgt.assign( src );
      return tgt;
   }

   template< class T,
             class U >
   std::vector<T>&
   assign( std::vector<T>& tgt,
           std::set<U>&& src )
   {
      tgt.resize( src.size() );
      std::copy( src.begin(), src.end(), tgt.begin() );
      hpc::deallocate( src );
      return tgt;
   }

   template< class T,
             class U >
   std::vector<T>&
   assign( std::vector<T>& tgt,
           std::set<U> const& src )
   {
      tgt.resize( src.size() );
      std::copy( src.begin(), src.end(), tgt.begin() );
      return tgt;
   }

}

#endif
