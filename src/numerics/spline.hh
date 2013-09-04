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

#include <boost/iterator/iterator_facade.hpp>
#include "libhpc/debug/debug.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/fibre.hh"
#include "libhpc/containers/functors.hh"
#include "tridiag.hh"

namespace hpc {
   namespace numerics {

      ///
      ///
      ///
      template< class > class spline_knot_iterator;

      ///
      ///
      ///
      template< class T,
                class PointsSeq = vector<T>,
                class ValuesSeq = vector<T> >
      class spline
      {
      public:

         typedef T value_type;
         typedef PointsSeq knot_points_type;
         typedef ValuesSeq knot_values_type;
         typedef spline_knot_iterator<value_type> knot_iterator;

      public:

         void
         set_knot_points( knot_points_type& pnts )
         {
            _pnts = pnts;
         }

         void
         set_knot_values( knot_values_type& vals )
         {
            _vals = val;
         }

         void
         update()
         {
            ASSERT( _pnts.size() == _vals.size(), "Mismatched points/values arrays in spline." );
            _diff.reallocate( _vals.size() - 1 );
            _ai.reallocate( _vals.size() - 1 );
            _bi.reallocate( _vals.size() - 1 );
            _solve();
         }

         const knot_points_type&
         knot_points_type() const
         {
            return _pnts;
         }

         const knot_values_type&
         knot_values_type() const
         {
            return _vals;
         }

         unsigned
         num_segments() const
         {
            return _diff.size();
         }

         value_type
         segment_start( unsigned seg ) const
         {
            ASSERT( seg < _diff.size() );
            return _knots(seg,0);
         }

         value_type
         segment_finish( unsigned seg ) const
         {
            ASSERT( seg < _diff.size() );
            return _knots(seg + 1,0);
         }

         value_type
         segment_width( unsigned seg ) const
         {
            ASSERT( seg < _diff.size() );
            return _knots(seg + 1,0) - _knots(seg,0);
         }

         knot_iterator
         abscissa_begin()
         {
            return knot_iterator( _knots.begin(), 0 );
         }

         knot_iterator
         abscissa_end()
         {
            return knot_iterator( _knots.end(), 0 );
         }

         knot_iterator
         values_begin()
         {
            return knot_iterator( _knots.begin(), 1 );
         }

         knot_iterator
         values_end()
         {
            return knot_iterator( _knots.end(), 1 );
         }

         value_type
         operator()( const value_type& crd ) const
         {
            typedef typename hpc::vector<value_type>::view array_type;

            typename fibre<value_type>::const_iterator it = std::lower_bound(
               _knots.begin(), _knots.end(), crd,
               compose2( std::less<value_type>(),
                         element<array_type>( 0 ),
                         identity<value_type>() )
               );

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
            {
               _diff[ii] = _knots(ii + 1,0) - _knots(ii,0);
               ASSERT( _diff[ii] > 2.0*std::numeric_limits<value_type>::epsilon() );
            }

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

         knot_values_type _vals;
         knot_points_type _pnts;
         hpc::vector<value_type> _diff;
         hpc::vector<value_type> _ai, _bi;
      };

      template< class T >
      class spline_knot_iterator
         : public boost::iterator_facade< spline_knot_iterator<T>,
                                          T,
                                          std::forward_iterator_tag,
                                          T& >
      {
         friend class boost::iterator_core_access;

      public:

         typedef T value_type;
         typedef value_type& reference_type;

      public:

         spline_knot_iterator()
         {
         }

         spline_knot_iterator( typename fibre<value_type>::iterator it,
                               unsigned idx )
            : _it( it ),
              _idx( idx )
         {
         }

         void
         increment()
         {
            ++_it;
         }

         bool
         equal( const spline_knot_iterator& op ) const
         {
            return _it == op._it;
         }

         reference_type
         dereference() const
         {
            return (*_it)[_idx];
         }

      protected:

         typename fibre<value_type>::iterator _it;
         unsigned _idx;
      };

   }
}

#endif
