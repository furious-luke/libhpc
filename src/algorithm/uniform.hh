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

#ifndef hpc_algorithm_uniform_hh
#define hpc_algorithm_uniform_hh

#include "libhpc/system/cc_version.hh"
#include "libhpc/system/cuda.hh"
#include "libhpc/debug/assert.hh"

namespace hpc {

   ///
   /// Lift implementation prototype.
   ///
   template< class ProjT,
             class LiftT,
             size_t D,
	     class SideT >
   struct lift_impl;

   ///
   /// 2D lift implementation.
   ///
   template< class ProjT,
             class LiftT,
	     class SideT >
   struct lift_impl<ProjT,LiftT,2,SideT>
   {
      CUDA_DEV_HOST
      static
      LiftT
      lift( ProjT idx,
            SideT const& sides )
      {
	 typedef typename LiftT::value_type lift_index_type;
         ASSERT( idx < sides[0]*sides[1], "Invalid grid cell index." );
         lift_index_type y = idx/sides[0];
#ifdef __CUDACC__
	 LiftT res = { (lift_index_type)(idx - y*sides[0]), y };
	 return res;
#else
	 return LiftT{ (lift_index_type)(idx - y*sides[0]), y };
#endif
      }
   };

   ///
   /// 3D lift implementation.
   ///
   template< class ProjT,
             class LiftT,
	     class SideT >
   struct lift_impl<ProjT,LiftT,3,SideT>
   {
      CUDA_DEV_HOST
      static
      LiftT
      lift( ProjT idx,
	    SideT const& sides )
      {
	 typedef typename LiftT::value_type lift_index_type;
         ASSERT( idx < sides[0]*sides[1]*sides[2], "Invalid grid cell index." );
         lift_index_type z = (lift_index_type)(idx/(sides[0]*sides[1]));
         ProjT rem = idx - z*(sides[0]*sides[1]);
         lift_index_type y = (lift_index_type)(rem/sides[0]);
#ifdef __CUDACC__
	 LiftT res = { (lift_index_type)(rem - y*sides[0]), y, z };
	 return res;
#else
         return LiftT{ (lift_index_type)(rem - y*sides[0]), y, z };
#endif
      }
   };

   ///
   /// Lift dispatch.
   ///
   template< class LiftT,
	     size_t D,
	     class ProjT,
	     class SideT >
   CUDA_DEV_HOST
   LiftT
   lift( ProjT idx,
	 SideT const& sides )
   {
      return lift_impl<ProjT,LiftT,D,SideT>::lift( idx, sides );
   }

   ///
   /// Project implementation prototype.
   ///
   template< class LiftT,
             class ProjT,
             size_t D,
	     class SideT >
   struct project_impl;

   ///
   /// 2D project implementation.
   ///
   template< class LiftT,
             class ProjT,
	     class SideT >
   struct project_impl<LiftT,ProjT,2,SideT>
   {
      CUDA_DEV_HOST
      static
      ProjT
      project( LiftT const& crd,
               SideT const& sides )
      {
#ifndef NDEBUG
         for( size_t ii = 0; ii < 2; ++ii )
            ASSERT( crd[ii] >= 0 && crd[ii] < sides[ii], "Invalid grid coordinate." );
#endif
         return crd[0] + crd[1]*sides[0];
      }
   };

   ///
   /// 3D project implementation.
   ///
   template< class LiftT,
             class ProjT,
	     class SideT >
   struct project_impl<LiftT,ProjT,3,SideT>
   {
      CUDA_DEV_HOST
      static
      ProjT
      project( LiftT const& crd,
               SideT const& sides )
      {
#ifndef NDEBUG
         for( size_t ii = 0; ii < 3; ++ii )
            ASSERT( crd[ii] >= 0 && crd[ii] < sides[ii], "Invalid grid coordinate." );
#endif
         return crd[0] + (crd[1] + crd[2]*sides[1])*sides[0];
      }
   };

   ///
   /// Project dispatch.
   ///
   template< class ProjT,
	     size_t D,
	     class LiftT,
	     class SideT >
   CUDA_DEV_HOST
   ProjT
   project( LiftT const& crd,
            SideT const& sides )
   {
      return project_impl<LiftT,ProjT,D,SideT>::project( crd, sides );
   }

#ifdef __CUDACC__

   namespace cuda {

      ///
      /// Cuda 3D lift implementation.
      ///
      CUDA_DEV_HOST_INL
      uint3
      lift( uint const& idx,
            uint3 const& sides )
      {
         ASSERT( idx < sides.x*sides.y*sides.z, "Invalid grid cell index." );
         uint z = idx/(sides.x*sides.y);
         uint rem = idx - z*(sides.x*sides.y);
         uint y = rem/sides.x;
         return make_uint3( rem - y*sides.x, y, z );
      }

      CUDA_DEV_HOST
      uint
      project( uint3 const& crd,
               uint3 const& sides )
      {
         ASSERT( crd.x >= 0 && crd.x < sides.x, "Invalid grid coordinate." );
         ASSERT( crd.y >= 0 && crd.y < sides.y, "Invalid grid coordinate." );
         ASSERT( crd.z >= 0 && crd.z < sides.z, "Invalid grid coordinate." );
         return crd.x + (crd.y + crd.z*sides.y)*sides.x;
      }

   }

#endif // __CUDACC__

}

#endif
