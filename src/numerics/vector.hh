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

#ifndef libhpc_numerics_vector_hh
#define libhpc_numerics_vector_hh

#ifdef HAVE_EIGEN

#include "eigen_vector.hh"

namespace hpc {
   namespace numerics {

      template< class T,
                int Rows=Eigen::Dynamic >  // TODO: fix this to be generic
      class vector
         : public impl::eigen::vector< T, Rows >
      {
      public:

         typedef impl::eigen::vector<T,Rows> super_type;

      public:

         vector()
            : super_type()
         {
         }
      };
   }
}

#endif

#endif
