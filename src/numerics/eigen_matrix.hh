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

#ifndef libhpc_numerics_eigen_matrix_hh
#define libhpc_numerics_eigen_matrix_hh

#ifdef HAVE_EIGEN

#include <Eigen/Eigen>
#include "eigen_vector.hh"

namespace hpc {
   namespace numerics {
      namespace impl {
         namespace eigen {

            template< class T,
                      int Rows,
                      int Cols >
            class matrix
               : public Eigen::Matrix< T, Rows, Cols >
            {
            public:

               typedef Eigen::Matrix<T,Rows,Cols> super_type;

            public:

               matrix()
                  : super_type()
               {
               }

               void
               solve( const vector<T,Rows>& rhs,
                      vector<T,Rows>& sol )
               {
                  // Need to cast the vector.
                  ((Eigen::Matrix<T,Rows,1>&)sol) = this->colPivHouseholderQr().solve( rhs );
               }
            };
         }
      }
   }
}

#endif

#endif
