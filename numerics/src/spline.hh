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

#ifndef libhpc_numerics_spline_hh
#define libhpc_numerics_spline_hh

#include "libhpc/debug/debug.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/fibre.hh"
#include "libhpc/containers/functors.hh"
#include "tridiag.hh"

class spline_suite;

namespace hpc {
   namespace numerics {

      ///
      ///
      ///
      template< class T >
      class spline
      {
         friend class ::spline_suite;

      public:

         typedef T value_type;

      public:

         void
         set_knots( fibre<value_type>& knots )
         {
            ASSERT( knots.fibre_size() == 2 );
            ASSERT( knots.size() > 1 );

            _knots.take( knots );
            _diff.reallocate( _knots.size() - 1 );
            _ai.reallocate( _knots.size() - 1 );
            _bi.reallocate( _knots.size() - 1 );
            _solve();
         }

         const fibre<value_type>&
         knots() const
         {
            return _knots;
         }

         value_type
         operator()( const value_type& crd ) const
         {
            typedef typename hpc::vector<value_type>::view array_type;
            auto it = std::lower_bound( _knots.begin(), _knots.end(), crd,
                                        compose2( std::less<value_type>(),
                                                  element<array_type>( 0 ),
                                                  identity<value_type>() ) );
            unsigned poly;
            if( it == _knots.end() )
               poly = _ai.size() - 1;
            else
            {
               poly = it - _knots.begin();
               if( poly > 0 )
                  --poly;
            }
            return _eval_poly( crd, poly );
         }

         value_type
         operator()( const value_type& crd,
                     unsigned poly ) const
         {
            return _eval_poly( crd, poly );
         }

      protected:

         value_type
         _eval_poly( const value_type& crd,
                     unsigned poly ) const
         {
            value_type t = (crd - _knots(poly,0))*_diff[poly];
            value_type u = 1.0 - t;
            return u*_knots(poly,1) + t*_knots(poly + 1,1) + t*u*(_ai[poly]*u + _bi[poly]*t);
         }

         void
         _solve()
         {
            hpc::vector<value_type> diag( _knots.size() ), off_diag( _knots.size() - 1 );
            hpc::vector<value_type> rhs( _knots.size() ), work( _knots.size() - 1 );
            hpc::vector<value_type> sol( _knots.size() );

            _assemble( diag, off_diag, rhs );
            tridiag_symm_solve<value_type>( diag, off_diag, rhs, sol, work );

            for( unsigned ii = 0; ii < _ai.size(); ++ii )
            {
               value_type tmp0 = _diff[ii];
               value_type tmp1 = _knots(ii + 1,1) - _knots(ii,1);
               _ai[ii] = sol[ii]*tmp0 - tmp1;
               _bi[ii] = -sol[ii + 1]*tmp0 + tmp1;
            }

            // Take reciprocal of differences for performance boost.
            for( unsigned ii = 0; ii < _diff.size(); ++ii )
               _diff[ii] = 1.0/_diff[ii];
         }

         void
         _assemble( hpc::vector<value_type>& diag,
                    hpc::vector<value_type>& off_diag,
                    hpc::vector<value_type>& rhs )
         {
            // Calculate differences first.
            for( unsigned ii = 0; ii < _diff.size(); ++ii )
               _diff[ii] = _knots(ii + 1,0) - _knots(ii,0);

            value_type prev = 2.0/_diff[0];
            diag[0] = prev;
            off_diag[0] = 0.5*prev;
            rhs[0] = 3.0*(_knots(1,1) - _knots(0,1))/(_diff[0]*_diff[0]);
            unsigned ii = 1;
            for( ; ii < off_diag.size(); ++ii )
            {
               value_type tmp = 2.0/_diff[ii];
               diag[ii] = prev + tmp;
               off_diag[ii] = 0.5*tmp;
               rhs[ii] = 3.0*((_knots(ii,1) - _knots(ii - 1,1))/(_diff[ii - 1]*_diff[ii - 1]) + (_knots(ii + 1,1) - _knots(ii,1))/(_diff[ii]*_diff[ii]));
               prev = tmp;
            }
            diag[ii] = prev;
            rhs[ii] = 3.0*(_knots(ii,1) - _knots(ii - 1,1))/(_diff[ii - 1]*_diff[ii - 1]);
         }

      protected:

         fibre<value_type> _knots;
         hpc::vector<value_type> _diff;
         hpc::vector<value_type> _ai, _bi;
      };
   }
}

#endif
