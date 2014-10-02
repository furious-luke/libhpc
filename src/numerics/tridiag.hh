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

#ifndef libhpc_numerics_tridiag_hh
#define libhpc_numerics_tridiag_hh

#include "libhpc/debug/assert.hh"

namespace hpc {
   namespace num {

      ///
      ///
      ///
      template< class DiagVec,
                class OffDiagVec = DiagVec,
                class RHSVec = DiagVec,
                class SolVec = DiagVec,
                class WorkVec = DiagVec >
      void
      tridiag_symm_solve( typename type_traits<DiagVec>::const_reference diag,
                          typename type_traits<OffDiagVec>::const_reference off_diag,
                          typename type_traits<RHSVec>::const_reference rhs,
                          typename type_traits<SolVec>::reference sol,
                          typename type_traits<WorkVec>::reference work )
      {
         typedef typename SolVec::value_type value_type;

         ASSERT( diag.size() > 1 );
         ASSERT( diag.size() == off_diag.size() + 1 );
         ASSERT( diag.size() == rhs.size() );
         ASSERT( diag.size() == sol.size() );
         ASSERT( diag.data() != sol.data() );
         ASSERT( off_diag.size() == work.size() );
         ASSERT( off_diag.data() != work.data() );

         // Forward sweep.
         sol[0] = rhs[0]/diag[0];
         work[0] = off_diag[0]/diag[0];
         unsigned ii = 1;
         for( ; ii < off_diag.size(); ++ii )
         {
            value_type tmp = 1.0/(diag[ii] - work[ii - 1]*off_diag[ii - 1]);
            sol[ii] = (rhs[ii] - sol[ii - 1]*off_diag[ii - 1])*tmp;
            work[ii] = off_diag[ii]*tmp;
         }
         sol[ii] = (rhs[ii] - sol[ii - 1]*off_diag[ii - 1])/(diag[ii] - work[ii - 1]*off_diag[ii - 1]);

         // Back-substitution.
         for( ; ii > 0; --ii )
            sol[ii - 1] -= work[ii - 1]*sol[ii];
      }

   }
}

#endif
