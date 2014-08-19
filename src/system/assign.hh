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

#ifndef hpc_system_assign_hh
#define hpc_system_assign_hh

#include <vector>
#include <set>
#include <boost/array.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_class.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include "libhpc/system/cc_version.hh"
#include "view.hh"
#include "deallocate.hh"

namespace hpc {

#ifdef CXX_0X

   // Assigning the same class.
   template< class T,
             class U,
             typename boost::enable_if<
                boost::is_same<typename boost::remove_reference<T>::type,
                               typename boost::remove_const<typename boost::remove_reference<U>::type>::type>,
                int>::type = 0 >
   inline
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
           view<std::vector<U>>&& src )
   {
      tgt.resize( src.size() );
      std::copy( src.begin(), src.end(), tgt.begin() );
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

   template< class T >
   view<std::vector<T> const>&
   assign( view<std::vector<T> const>& tgt,
           view<std::vector<T> >&& src )
   {
      tgt.assign( src );
      return tgt;
   }

   template< class T >
   view<std::vector<T> const>&
   assign( view<std::vector<T> const>& tgt,
           view<std::vector<T> const> const& src )
   {
      tgt.assign( src );
      return tgt;
   }

#endif

   /// Non-class types.
   template< class T,
             class U >
   inline
   T&
   assign( T& tgt,
           U const& src,
           typename boost::disable_if<boost::is_class<T>,int>::type = 0 )
   {
      tgt = src;
      return tgt;
   }

   template< class T,
             class U >
   std::vector<T>&
   assign( std::vector<T>& tgt,
           view<std::vector<U> > const& src )
   {
      tgt.resize( src.size() );
      std::copy( src.begin(), src.end(), tgt.begin() );
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

   ///
   /// Adopt source vector.
   ///
   /// @param tgt Target view to adopt values.
   /// @param src Source vector to provide values.
   /// @return Reference to target view.
   ///
   template< class T >
   view<std::vector<T> >&
   assign( view<std::vector<T> >& tgt,
           std::vector<T> const& src )
   {
      tgt.assign( src );
      return tgt;
   }

   template< class T >
   view<std::vector<T> const>&
   assign( view<std::vector<T> const>& tgt,
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
   view<std::vector<T> >&
   assign( view<std::vector<T> >& tgt,
           std::vector<T>& src )
   {
      tgt.assign( src );
      return tgt;
   }

   template< class T,
             class U,
             size_t N >
   view<std::vector<T> >&
   assign( view<std::vector<T> >& tgt,
           boost::array<U,N> const& src )
   {
      ASSERT( tgt.size() == N, "Incompatible view and array sizes (", tgt.size(), " and ", N, ")." );
      std::copy( src.begin(), src.end(), tgt.begin() );
      return tgt;
   }

   template< class T,
             size_t N,
             class U >
   boost::array<T,N>&
   assign( boost::array<T,N>& tgt,
           view<std::vector<U> > const& src )
   {
      ASSERT( tgt.size() == N, "Incompatible view and array sizes (", tgt.size(), " and ", N, ")." );
      std::copy( src.begin(), src.end(), tgt.begin() );
      return tgt;
   }

}

#endif
