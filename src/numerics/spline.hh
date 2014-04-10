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

#include <vector>
#include <boost/iterator/iterator_facade.hpp>
#include "libhpc/debug/assert.hh"
#include "libhpc/system/assign.hh"
#include "libhpc/system/view.hh"
#include "tridiag.hh"

namespace hpc {
   namespace num {

      ///
      ///
      ///
      template< class T,
                class PointsSeq = std::vector<T>,
                class ValuesSeq = std::vector<T> >
      class spline
      {
      public:

         typedef T value_type;
         typedef PointsSeq knot_points_type;
         typedef ValuesSeq knot_values_type;

      public:

         template< class Seq >
         void
         set_knot_points( Seq&& pnts )
         {
            assign( _pnts, std::move( pnts ) );
         }

         template< class Seq >
         void
         set_knot_points( Seq const& pnts )
         {
            assign( _pnts, pnts );
         }

         template< class Seq >
         void
         set_knot_values( Seq&& vals )
         {
            assign( _vals, std::move( vals ) );
         }

         template< class Seq >
         void
         set_knot_values( Seq const& vals )
         {
            assign( _vals, vals );
         }

         void
         update()
         {
            ASSERT( _pnts.size() == _vals.size(), "Mismatched points/values arrays in spline." );
	    ASSERT( _pnts.size() > 1, "Too few knots for spline." );
            _diff.resize( _vals.size() - 1 );
            _ai.resize( _vals.size() - 1 );
            _bi.resize( _vals.size() - 1 );
            _solve();
         }

         const knot_points_type&
         knot_points() const
         {
            return _pnts;
         }

         const knot_values_type&
         knot_values() const
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
            return _pnts[seg];
         }

         value_type
         segment_finish( unsigned seg ) const
         {
            return _pnts[seg + 1];
         }

         value_type
         segment_width( unsigned seg ) const
         {
            return _pnts[seg + 1] - _pnts[seg];
         }

         knot_points_type&
         points()
         {
            return _pnts;
         }

         knot_values_type&
         values()
         {
            return _vals;
         }

         typename knot_points_type::const_iterator
         points_begin() const
         {
            return _pnts.cbegin();
         }

         typename knot_points_type::const_iterator
         points_end() const
         {
            return _pnts.cend();
         }

         typename knot_values_type::const_iterator
         values_begin() const
         {
            return _vals.cbegin();
         }

         unsigned
         poly( value_type const& crd ) const
         {
            unsigned poly;
            auto it = std::lower_bound( _pnts.begin(), _pnts.end(), crd );
            if( it == _pnts.end() )
               poly = _ai.size() - 1;
            else
            {
               poly = it - _pnts.begin();
               if( poly > 0 )
                  --poly;
            }
            return poly;
         }

         value_type
         operator()( const value_type& crd ) const
         {
            unsigned poly = this->poly( crd );
            return _eval_poly( crd, poly );
         }

         value_type
         operator()( const value_type& crd,
                     unsigned poly ) const
         {
            return _eval_poly( crd, poly );
         }

         typename hpc::view<std::vector<value_type>> const
         ai() const
         {
            return _ai;
         }

         typename hpc::view<std::vector<value_type>> const
         bi() const
         {
            return _bi;
         }

      protected:

         value_type
         _eval_poly( const value_type& crd,
                     unsigned poly ) const
         {
            value_type t = (crd - _pnts[poly])*_diff[poly];
            value_type u = 1.0 - t;
            return u*_vals[poly] + t*_vals[poly + 1] + t*u*(_ai[poly]*u + _bi[poly]*t);
         }

         void
         _solve()
         {
            std::vector<value_type> diag( _pnts.size() ), off_diag( _pnts.size() - 1 );
            std::vector<value_type> rhs( _pnts.size() ), work( _pnts.size() - 1 );
            std::vector<value_type> sol( _pnts.size() );

            _assemble( diag, off_diag, rhs );
            tridiag_symm_solve<std::vector<value_type>>( diag, off_diag, rhs, sol, work );

            for( unsigned ii = 0; ii < _ai.size(); ++ii )
            {
               value_type tmp0 = _diff[ii];
               value_type tmp1 = _vals[ii + 1] - _vals[ii];
               _ai[ii] = sol[ii]*tmp0 - tmp1;
               _bi[ii] = -sol[ii + 1]*tmp0 + tmp1;
            }

            // Take reciprocal of differences for performance boost.
            for( unsigned ii = 0; ii < _diff.size(); ++ii )
               _diff[ii] = 1.0/_diff[ii];
         }

         void
         _assemble( std::vector<value_type>& diag,
                    std::vector<value_type>& off_diag,
                    std::vector<value_type>& rhs )
         {
            // Calculate differences first.
            for( unsigned ii = 0; ii < _diff.size(); ++ii )
            {
               _diff[ii] = _pnts[ii + 1] - _pnts[ii];
               ASSERT( _diff[ii] > 2.0*std::numeric_limits<value_type>::epsilon(),
                       "Can't assemble spline system with aliased points." );
            }

            value_type prev = 2.0/_diff[0];
            diag[0] = prev;
            off_diag[0] = 0.5*prev;
            rhs[0] = 3.0*(_vals[1] - _vals[0])/(_diff[0]*_diff[0]);
            unsigned ii = 1;
            for( ; ii < off_diag.size(); ++ii )
            {
               value_type tmp = 2.0/_diff[ii];
               diag[ii] = prev + tmp;
               off_diag[ii] = 0.5*tmp;
               rhs[ii] = 3.0*((_vals[ii] - _vals[ii - 1])/(_diff[ii - 1]*_diff[ii - 1]) + 
                              (_vals[ii + 1] - _vals[ii])/(_diff[ii]*_diff[ii]));
               prev = tmp;
            }
            diag[ii] = prev;
            rhs[ii] = 3.0*(_vals[ii] - _vals[ii - 1])/(_diff[ii - 1]*_diff[ii - 1]);
         }

      protected:

         knot_values_type _vals;
         knot_points_type _pnts;
         std::vector<value_type> _diff;
         std::vector<value_type> _ai, _bi;
      };

   }
}

#endif
