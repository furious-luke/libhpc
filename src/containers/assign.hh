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

#ifndef libhpc_containers_assign_hh
#define libhpc_containers_assign_hh

#include <vector>
#include "vector.hh"
#include "view.hh"

namespace hpc {

   template< class T,
             class U >
   T&
   assign( T& tgt,
           const U& src )
   {
      return tgt = src;
   }

   template< class T,
             class U >
   std::vector<T>&
   assign( std::vector<T>& tgt,
           const vector_view<U>& src )
   {
      tgt.resize( src.size() );
      std::copy( src.begin(), src.end(), tgt.begin() );
      return tgt;
   }

   template< class T,
             class U >
   std::vector<T>&
   assign( std::vector<T>& tgt,
           vector_view<U>&& src )
   {
      tgt.resize( src.size() );
      std::copy( src.begin(), src.end(), tgt.begin() );
      return tgt;
   }

   // template< class Target,
   //           class Source >
   // struct assign
   // {
   //    static
   //    Target&
   //    _( Target& tgt,
   //       const Source& src )
   //    {
   //       return tgt = src;
   //    }

   //    static
   //    Target&
   //    _( Target& tgt,
   //       Source&& src )
   //    {
   //       return tgt = src;
   //    }
   // };

   // template< class T,
   //           class U >
   // struct assign< std::vector<T>,
   //                vector_view<U> >
   // {
   //    static
   //    std::vector<T>&
   //    _( std::vector<T>& tgt,
   //       const vector_view<U>& src )
   //    {
   //       tgt.resize( src.size() );
   //       std::copy( src.begin(), src.end(), tgt.begin() );
   //       return tgt;
   //    }

   //    static
   //    std::vector<T>&
   //    _( std::vector<T>& tgt,
   //       vector_view<U>&& src )
   //    {
   //       tgt.resize( src.size() );
   //       std::copy( src.begin(), src.end(), tgt.begin() );
   //       return tgt;
   //    }
   // };

}

#endif
