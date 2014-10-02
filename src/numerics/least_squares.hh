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

#ifndef libhpc_numerics_least_squares_hh
#define libhpc_numerics_least_squares_hh

#ifdef HAVE_EIGEN

#include <vector>
#include <Eigen/Dense>
#include "libhpc/debug/assert.hh"
#include "libhpc/containers/grid.hh"
#include "libhpc/system/math.hh"

namespace hpc {
   namespace numerics {

      template< class T = double >
      class least_squares
      {
      public:

         typedef T value_type;

      public:

         least_squares()
            : _dim( 0 ),
              _ord( 0 ),
              _ns( 0 )
         {
         }

         void
         resize( unsigned num_smpls,
                 unsigned ord = 1,
                 unsigned dim = 1 )
         {
            _dim = dim;
            _ord = ord;
            _nterms = powi( _ord + 1, _dim );
            _ns = num_smpls;
            _terms.resize( dim, ord + 1 );
            _pos = 0;
            _smpls.resize( num_smpls, _nterms );
            _vals.resize( num_smpls );
            _design.resize( _nterms, _nterms );
            _rhs.resize( _nterms );
            _sol.resize( _nterms );
         }

         void
         reset()
         {
            _pos = 0;
         }

         template< class Iter >
         void
         add_sample( Iter const& begin,
                     value_type val )
         {
            std::vector<unsigned> crds( _dim );
            for( unsigned ii = 0; ii < _terms.size(); ++ii )
            {
               value_type cur = 1.0;
               _terms.lift( ii, crds.begin() );
               Iter cur_it = begin;
               for( unsigned jj = 0; jj < _dim; ++jj )
                  cur *= pow( *cur_it++, crds[jj] );
               _smpls( _pos, ii ) = cur;
            }
            _vals( _pos ) = val;
            ++_pos;
         }

         void
         add_sample( value_type x,
                     value_type val )
         {
            ASSERT( _dim == 1 );
            add_sample( &x, val );
         }

         void
         solve()
         {
            _design = _smpls.transpose()*_smpls;
            _rhs = _smpls.transpose()*_vals;
            _sol = _design.jacobiSvd( Eigen::ComputeThinU | Eigen::ComputeThinV ).solve( _rhs );
         }

         Eigen::Matrix<value_type, Eigen::Dynamic, 1> const&
         solution() const
         {
            return _sol;
         }

      protected:

         unsigned _ord, _ns, _nterms, _pos, _dim;
         hpc::grid<unsigned> _terms;
         Eigen::Matrix<value_type, Eigen::Dynamic, Eigen::Dynamic> _design, _smpls;
         Eigen::Matrix<value_type, Eigen::Dynamic, 1> _rhs, _sol, _vals;
      };

   }
}

#endif

#endif
