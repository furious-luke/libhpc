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

#ifndef libhpc_numerics_spline_integrator_hh
#define libhpc_numerics_spline_integrator_hh

#include "quadrature.hh"
#include "interp_iterator.hh"

namespace hpc {
   namespace num {

      template< class T >
      class spline_integrator
      {
      public:

         typedef T value_type;

      public:

         spline_integrator( unsigned order = 3 )
         {
            make_quadrature( gauss_legendre_generator<double>( -1.0, 1.0 ), order, _quad );
         }

         template< class Spline,
                   class Func >
         value_type
         operator()( const Spline& sp,
                     Func func ) const
         {
            value_type sum = 0.0;
            for( unsigned ii = 0; ii < sp.num_segments(); ++ii )
            {
               value_type low = sp.segment_start( ii );
               value_type w = sp.segment_width( ii );
               value_type jac_det = 0.5*w;
               value_type cur_sum = 0.0;
               for( unsigned jj = 0; jj < _quad.size(); ++jj )
               {
                  value_type x = low + w*0.5*(1.0 + _quad.points()[jj]);
                  cur_sum += _quad.weights()[jj]*func( x, sp( x, ii ) );
               }
               cur_sum *= jac_det;
               sum += cur_sum;
            }
            return sum;
         }

         template< class Spline,
                   class Func,
		   class Trans >
         value_type
         operator()( const Spline& sp,
                     Func func,
		     Trans trans ) const
         {
            value_type sum = 0.0;
            for( unsigned ii = 0; ii < sp.num_segments(); ++ii )
            {
               value_type low = sp.segment_start( ii );
	       value_type upp = sp.segment_finish( ii );
	       value_type w = upp - low;
               value_type jac_det = 0.5*(trans( upp ) - trans( low ));
               value_type cur_sum = 0.0;
               for( unsigned jj = 0; jj < _quad.size(); ++jj )
               {
                  value_type x = low + w*0.5*(1.0 + _quad.points()[jj]);
                  cur_sum += _quad.weights()[jj]*func( trans( x ), sp( x, ii ) );
               }
               cur_sum *= jac_det;
               sum += cur_sum;
            }
            return sum;
         }

         template< class Spline,
                   class Func >
         value_type
         reverse( const Spline& sp,
		  Func func ) const
         {
            value_type sum = 0.0;
            for( unsigned ii = 0; ii < sp.num_segments(); ++ii )
            {
	       unsigned idx = sp.num_segments() - ii - 1;
               value_type low = sp.segment_start( idx );
               value_type w = sp.segment_width( idx );
               value_type jac_det = 0.5*w;
               value_type cur_sum = 0.0;
               for( unsigned jj = 0; jj < _quad.size(); ++jj )
               {
		  unsigned jj_idx = _quad.size() - jj - 1;
                  value_type x = low + w*0.5*(1.0 + _quad.points()[jj_idx]);
                  cur_sum += _quad.weights()[jj]*func( x, sp( x, idx ) );
               }
               cur_sum *= jac_det;
               sum += cur_sum;
            }
            return sum;
         }

      protected:

         quadrature<value_type> _quad;
      };

      template< class T >
      class spline_spline_integrator
      {
      public:

         typedef T value_type;

      public:

         spline_spline_integrator( unsigned order = 6 )
         {
            make_quadrature( gauss_legendre_generator<double>( -1.0, 1.0 ), order, _quad );
         }

         template< class Spline0,
                   class Spline1,
                   class Func >
         value_type
         operator()( const Spline0& sp0,
                     const Spline1& sp1,
                     Func func ) const
         {
            // Find the range of overlap.
            value_type low = std::max( sp0.knot_points().front(), sp1.knot_points().front() );
            value_type upp = std::min( sp0.knot_points().back(),  sp1.knot_points().back() );

            // If there is no overlap, return 0.
            if( upp <= low )
               return 0.0;

            // Make the interpolation iterator.
            auto it = make_interp_iterator(
               sp0.knot_points().begin(),
               sp0.knot_points().end(),
               sp1.knot_points().begin(),
               sp1.knot_points().end(),
               1e-7
               );

            // Skip anything outside of the overlap range.
            while( !approx( *it, low, 1e-7 ) )
               ++it;

            // Sum until we hit the upper limit.
            value_type sum = 0.0;
            while( !approx( *it++, upp, 1e-7 ) )
            {
               value_type w = *it - low;
               value_type jac_det = 0.5*w;
               unsigned sp0_poly = it.indices()[0] - 1;
               unsigned sp1_poly = it.indices()[1] - 1;
               value_type cur_sum = 0.0;
               for( unsigned ii = 0; ii < _quad.size(); ++ii )
               {
                  value_type x = low + w*0.5*(1.0 + _quad.points()[ii]);
                  cur_sum += _quad.weights()[ii]*func( x, sp0( x, sp0_poly ), sp1( x, sp1_poly ) );
               }
               cur_sum *= jac_det;
               sum += cur_sum;
               low = *it;
            }

            return sum;
         }

      protected:

         quadrature<value_type> _quad;
      };

   }
}

#endif
