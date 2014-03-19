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

#ifndef libhpc_algorithm_uniform_hh
#define libhpc_algorithm_uniform_hh

#include <stdint.h>
#include "libhpc/debug/assert.hh"
#include "libhpc/containers/array.hh"

namespace hpc {

   ///
   /// Lift implementation prototype.
   ///
   template< class IndexP,
             class IndexL,
             unsigned D >
   struct lift_impl;

   ///
   /// 2D lift implementation.
   ///
   template< class IndexP,
             class IndexL >
   struct lift_impl<IndexP,IndexL,2>
   {
      static
      hpc::array<IndexL,2>
      lift( IndexP idx,
            hpc::array<IndexL,2> const& sides )
      {
         ASSERT( idx < sides[0]*sides[1], "Invalid grid cell index." );
         IndexL y = idx/sides[0];
         return hpc::array<IndexL,2>{ static_cast<IndexL>( idx - y*sides[0] ), y };
      }
   };

   ///
   /// 3D lift implementation.
   ///
   template< class IndexP,
             class IndexL >
   struct lift_impl<IndexP,IndexL,3>
   {
      static
      hpc::array<IndexL,3>
      lift( IndexP idx,
            hpc::array<IndexL,3> const& sides )
      {
         ASSERT( idx < sides[0]*sides[1]*sides[2], "Invalid grid cell index." );
         IndexL z = static_cast<IndexL>( idx/(sides[0]*sides[1]) );
         IndexP rem = idx - z*(sides[0]*sides[1]);
         IndexL y = static_cast<IndexL>( rem/sides[0] );
         return hpc::array<IndexL,3>{ static_cast<IndexL>( rem - y*sides[0] ), y, z };
      }
   };

   ///
   /// Lift dispatch.
   ///
   template< class IndexP,
             class IndexL,
             unsigned D >
   hpc::array<IndexL,D>
   lift( IndexP idx,
         hpc::array<IndexL,D> const& sides )
   {
      return lift_impl<IndexP,IndexL,D>::lift( idx, sides );
   }

   ///
   /// Project implementation prototype.
   ///
   template< class IndexL,
             class IndexP,
             unsigned D >
   struct project_impl;

   ///
   /// 2D project implementation.
   ///
   template< class IndexL,
             class IndexP >
   struct project_impl<IndexL,IndexP,2>
   {
      static
      IndexP
      project( hpc::array<IndexL,2> const& crd,
               hpc::array<IndexL,2> const& sides )
      {
#ifndef NDEBUG
         for( unsigned ii = 0; ii < 2; ++ii )
            ASSERT( crd[ii] >= 0 && crd[ii] < sides[ii], "Invalid grid coordinate." );
#endif
         return crd[0] + crd[1]*sides[0];
      }
   };

   ///
   /// 3D project implementation.
   ///
   template< class IndexL,
             class IndexP >
   struct project_impl<IndexL,IndexP,3>
   {
      static
      IndexP
      project( hpc::array<IndexL,3> const& crd,
               hpc::array<IndexL,3> const& sides )
      {
#ifndef NDEBUG
         for( unsigned ii = 0; ii < 3; ++ii )
            ASSERT( crd[ii] >= 0 && crd[ii] < sides[ii], "Invalid grid coordinate." );
#endif
         return crd[0] + (crd[1] + crd[2]*sides[1])*sides[0];
      }
   };

   ///
   /// Project dispatch.
   ///
   template< class IndexL,
             class IndexP,
             unsigned D >
   IndexP
   project( hpc::array<IndexL,D> const& crd,
            hpc::array<IndexL,D> const& sides )
   {
      project_impl<IndexL,IndexP,D>::project( crd, sides );
   }

}

#endif
