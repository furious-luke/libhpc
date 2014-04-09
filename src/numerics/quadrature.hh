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

#ifndef libhpc_numerics_quadrature_hh
#define libhpc_numerics_quadrature_hh

#include <math.h>
#include <vector>
#include <boost/math/special_functions.hpp>
#include "libhpc/system/math.hh"
#include "libhpc/algorithm/newton.hh"
#include "polynomial.hh"

using boost::math::lgamma;

namespace hpc {
   namespace num {

      ///
      /// Calculate the Gauss-Legendre quadrature.
      ///
      /// @param[in]  order     The order of the Legendre polynomials to use.
      /// @param[in]  x1        Lower bracket.
      /// @param[in]  x2        Upper bracket.
      /// @param[out] points    The generated abscissa.
      /// @param[out] weights   The generated weights.
      /// @param[in]  tolerance The accuracy.
      ///
      template< class T,
                class PointIter,
                class WeightIter >
      void
      gauss_legendre( unsigned size,
                      T x1,
                      T x2,
                      PointIter points,
                      WeightIter weights,
                      T tolerance = num::default_newton_tolerance )
      {
         // Gauss-Legendre polynomial roots are symmetric; we only need
         // to calculate half of them.
         unsigned np = size;
         unsigned half = (np + 1)/2;

         // Need to cache the first half of the points.
         std::vector<T> cache( 2*half );

         // Construct the bracket for the half range.
         T xl = 0.5*(x2 - x1);
         T xu = 0.5*(x2 + x1);

         // Setup the polynomial functor for the Legendre polynomials.
         polynomial::legendre<T> legendre( np );

         // Calculate the root for each unique.
         T x, df;
         for( unsigned ii = 1; ii <= half; ++ii )
         {
            // Initial guess for this root.
            x = cos( M_PI*(ii - 0.25)/(np + 0.5) );

            // Solve using Newton-Raphson.
            x = num::newton<polynomial::legendre<T>, T>( legendre, xl, xu, x, df, tolerance );

            *points++ = xu - xl*x;
            cache[2*(ii - 1)] = xu - xl*x;
            *weights++ = 2.0*xl/((1.0 - x*x)*df*df);
            cache[2*(ii - 1) + 1] = 2.0*xl/((1.0 - x*x)*df*df);
         }

         // Copy the first half of the values to the second.
         half = np/2;
         for( unsigned ii = 0; ii < half; ++ii )
         {
            *points++ = -cache[2*(half - ii - 1)];
            *weights++ = cache[2*(half - ii - 1) + 1];
         }
      }

      template< class T >
      class gauss_legendre_generator
      {
      public:

         gauss_legendre_generator( T x0 = -1.0,
                                   T x1 = 1.0 )
            : _x0( x0 ),
              _x1( x1 )
         {
         }

         template< class PointIter,
                   class WeightIter >
         void
         operator()( unsigned size,
                     PointIter points,
                     WeightIter weights,
                     T tolerance = 1e-8 ) const
         {
            gauss_legendre( size, _x0, _x1, points, weights, tolerance );
         }

      private:

         T _x0;
         T _x1;
      };

      ///
      /// Calculate the Gauss-Jacobi quadrature.
      ///
      /// @param[in]  order     The order of the Legendre polynomials to use.
      /// @param[in]  alpha     ?
      /// @param[in]  beta      ?
      /// @param[out] points    The generated abscissa.
      /// @param[out] weights   The generated weights.
      /// @param[in]  tolerance The accuracy.
      ///
      template< class T,
                class PointIter,
                class WeightIter >
      void
      gauss_jacobi_ref( unsigned size,
                        T alpha,
                        T beta,
                        PointIter& points,
                        WeightIter& weights,
                        T tolerance = 1e-8 )
      {
         // Setup the polynomial functor for the Legendre polynomials.
         unsigned np = size;
         polynomial::jacobi<T> jacobi( np, alpha, beta );

         // Need to keep 3 previous evaluations.
         T prev[3];

         // Calculate each of the roots.
         T x, df;
         T an, bn;
         T r1, r2, r3;
         for( unsigned ii = 0; ii < np; ++ii )
         {
            // The initial guesses are a bit complicated.
            if( ii == 0 )
            {
               an = alpha/np;
               bn = beta/np;
               r1 = (1.0 + alpha)*(2.78/(4.0 + np*np) + 0.768*an/np);
               r2 = 1.0 + 1.48*an + 0.96*bn + 0.452*an*an + 0.83*an*bn;
               x = 1.0 - r1/r2;
            }
            else if( ii == 1 )
            {
               r1 = (4.1 + alpha)/((1.0 + alpha)*(1.0 + 0.156*alpha));
               r2 = 1.0 + 0.06*(np - 8.0)*(1.0 + 0.12*alpha)/np;
               r3 = 1.0 + 0.012*beta*(1.0 + 0.25*fabs(alpha))/np;
               x -= (1.0 - x)*r1*r2*r3;
            }
            else if( ii == 2 )
            {
               r1 = (1.67 + 0.28*alpha)/(1.0 + 0.37*alpha);
               r2 = 1.0 + 0.22*(np - 8.0)/np;
               r3 = 1.0 + 8.0*beta/((6.28 + beta)*np*np);
               x -= (prev[1] - x)*r1*r2*r3;
            }
            else if( ii == np - 2 )
            {
               r1 = (1.0 + 0.235*beta)/(0.766 + 0.119*beta);
               r2 = 1.0/(1.0 + 0.639*(np - 4.0)/(1.0 + 0.71*(np - 4.0)));
               r3 = 1.0/(1.0 + 20.0*alpha/((7.5 + alpha)*np*np));
               x += (x - prev[1])*r1*r2*r3;
            }
            else if( ii == np - 1 )
            {
               r1 = (1.0 + 0.37*beta)/(1.67 + 0.28*beta);
               r2 = 1.0/(1.0 + 0.22*(np - 8.0)/np);
               r3 = 1.0/(1.0 + 8.0*alpha/((6.28 + alpha)*np*np));
               x += (x - prev[1])*r1*r2*r3;
            }
            else
            {
               x = 3.0*prev[0] - 3.0*prev[1] + prev[2];
            }

            // Solve using Newton-Raphson.
            x = num::newton<polynomial::jacobi<T>, T>( jacobi, -1.0, 1.0, x, df, tolerance );

            // Update previous values.
            prev[2] = prev[1];
            prev[1] = prev[0];
            prev[0] = x;

            *points++ = -x;
            *weights++ = exp( lgamma<T>( alpha + np ) + lgamma<T>( beta + np ) - lgamma<T>( np + 1.0 ) - 
                              lgamma<T>( np + alpha + beta + 1.0 ))*(2*np + alpha + beta)*pow( 2.0, alpha + beta )/( df*jacobi.previous_order() );
         }

         // // Reverse the order of the points.
         // // TODO: Should incorprate this above.
         // for( int ii = 0; ii < points.size(); ++ii )
         //    points[ii] *= -1.0;
      }

      template< class T,
                class PointIter,
                class WeightIter >
      void
      gauss_jacobi( unsigned size,
                    T alpha,
                    T beta,
                    PointIter points,
                    WeightIter weights,
                    T tolerance = 1e-8 )
      {
         gauss_jacobi_ref( size, alpha, beta, points, weights, tolerance );
      }

      ///
      /// Calculate the Gauss-Lobatto quadrature.
      ///
      /// @param[in]  order     The order of the Legendre polynomials to use.
      /// @param[in]  alpha     ?
      /// @param[in]  beta      ?
      /// @param[out] points    The generated abscissa.
      /// @param[out] weights   The generated weights.
      /// @param[in]  tolerance The accuracy.
      ///
      template< class T,
                class PointIter,
                class WeightIter >
      void
      gauss_lobatto( unsigned size,
                     T alpha,
                     T beta,
                     PointIter points,
                     WeightIter weights,
                     T tolerance = 1e-8 )
      {
         ASSERT( size >= 2, "Too few points for Gauss-Lobatto quadrature." );

         // Set initial value.
         *points++ = -1.0;
         ++weights;

         // Only calculate interior if needed.
         if( size > 2 )
         {
            // Calculate the interior points and the weights.
            gauss_jacobi_ref(
               size,
               alpha + 1.0,
               beta + 1.0,
               points,
               weights,
               tolerance
            );
         }

         // Set final point.
         *points = -1.0;
      }

      // template< class T >
      // class gauss_lobatto_gen
      // {
      // public:

      //    gauss_lobatto_gen( T alpha = 0.0,
      //                       T beta = 0.0 )
      //       : _alpha( alpha ),
      //         _beta( beta )
      //    {
      //    }

      //    ~gauss_lobatto_gen()
      //    {
      //    }

      //    void
      //    operator()( typename vector<T>::view points,
      //                typename vector<T>::view weights,
      //                T tolerance = 1e-8 ) const
      //    {
      //       gauss_lobatto( this->_alpha, this->_beta, points, weights, tolerance );
      //    }

      // private:

      //    T _alpha;
      //    T _beta;
      // };

      template< class Points1DIter,
                class Weights1DIter,
                class PointsOutIter,
                class WeightsOutIter >
      void
      outer_product( unsigned size_1d,
                     unsigned dim,
                     Points1DIter points_1d,
                     Weights1DIter weights_1d,
                     PointsOutIter points,
                     WeightsOutIter weights )
      {
         typedef typename WeightsOutIter::value_type value_type;

         grid<unsigned> grid;
         grid.setup_block( dim, size_1d );

         std::vector<unsigned> indices( dim );
         for( unsigned ii = 0; ii < grid.size(); ++ii )
         {
            grid.lift( ii, indices.begin() );
            value_type cur_w = 1.0;
            for( unsigned jj = 0; jj < dim; ++jj )
            {
               *points++ = points_1d[indices[jj]];
               cur_w *= weights_1d[indices[jj]];
            }
            *weights++ = cur_w;
         }
      }

      template< class Generator,
                class PointIter,
                class WeightIter >
      void
      make_quadrature( Generator gen,
                       unsigned order,
                       PointIter points,
                       WeightIter weights,
                       unsigned dim = 1,
                       typename PointIter::value_type tolerance = 1e-8 )
      {
         std::vector<double> points_1d( order + 1 );
         std::vector<double> weights_1d( order + 1 );
         gen( order + 1, points_1d.begin(), weights_1d.begin(), tolerance );
         outer_product( order + 1, dim, points_1d.begin(), weights_1d.begin(), points, weights );
      }

      template< class T >
      class quadrature
      {
      public:

         typedef T value_type;

      public:

         template< class Seq >
         void
         set_points( Seq& pnts )
         {
            _pnts = std::move( pnts );
         }

         template< class Seq >
         void
         set_points( const Seq& pnts )
         {
            _pnts = pnts;
         }

         template< class Seq >
         void
         set_weights( Seq& wgts )
         {
            _wgts = std::move( wgts );
         }

         template< class Seq >
         void
         set_weights( const Seq& wgts )
         {
            _wgts = wgts;
         }

         size_t
         size() const
         {
            return _wgts.size();
         }

         const std::vector<value_type>&
         points() const
         {
            return _pnts;
         }

         const std::vector<value_type>&
         weights() const
         {
            return _wgts;
         }

      protected:

         std::vector<value_type> _pnts;
         std::vector<value_type> _wgts;
      };

      template< class Generator,
                class T >
      void
      make_quadrature( Generator gen,
                       unsigned order,
                       quadrature<T>& quad,
                       unsigned dim = 1 )
      {
         unsigned size = powi( order + 1, dim );
         std::vector<double> points( size );
         std::vector<double> weights( size );
         make_quadrature( gen, order, points.begin(), weights.begin(), dim );
         quad.set_points( points );
         quad.set_weights( weights );
      }

   }
}

#endif
