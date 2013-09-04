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

#ifndef libhpc_numerics_polynomial_hh
#define libhpc_numerics_polynomial_hh

#include <math.h>
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/grid.hh"

namespace hpc {
   namespace polynomial {

      template< class T >
      class legendre
      {
      public:

         typedef T value_type;

      public:

         legendre( unsigned order )
            : _order( order )
         {
         }

         value_type
         operator()( const value_type& x )
         {
            // Sum up the value.
            T p0, p1, p2;
            p0 = 1.0;
            p1 = 0.0;
            for( unsigned ii = 1; ii <= this->_order; ++ii ) {
               p2 = p1;
               p1 = p0;
               p0 = ((2.0*ii - 1.0)*x*p1 - (ii - 1.0)*p2)/ii;
            }

            // Store the previous order evaluation.
            this->_p1 = p1;

            return p0;
         }

         value_type
         derivative( const value_type& x,
                     const value_type& value ) const
         {
            return _order*(x*value - _p1)/(x*x - 1.0);
         }

         T
         previous_order() const
         {
            return this->_p1;
         }

      private:

         unsigned _order;
         T _p1;
      };

      template< class T >
      class jacobi
      {
      public:

         typedef T value_type;

         jacobi( value_type alpha,
                 value_type beta )
            : _order( 1 ),
              _alpha( alpha ),
              _beta( beta )
         {
         }

         jacobi( unsigned order,
                 value_type alpha,
                 value_type beta )
            : _order( order ),
              _alpha( alpha ),
              _beta( beta )
         {
         }

         void
         set_order( unsigned order )
         {
            this->_order = order;
         }

         unsigned
         order() const
         {
            return this->_order;
         }

         value_type
         operator()( unsigned order,
                     const value_type& x )
         {
            // Catch zeroth order. Need this because the loop begins with
            // first order.
            if( order == 0 )
            {
               this->_p1 = 0.0;
               this->_tmp = 0.0;
               return 1.0;
            }

            // Cache some stuff.
            value_type al = this->_alpha;
            value_type be = this->_beta;
            value_type ab = al + be;

            value_type a, b, c;
            value_type tmp = 2.0 + ab;
            value_type p0 = (al - be + tmp*x)/2.0;
            value_type p1 = 1.0;
            value_type p2;
            for( unsigned ii = 2; ii <= order; ++ii )
            {
               p2 = p1;
               p1 = p0;
               tmp = 2*ii + ab;
               a = 2*ii*(ii + ab)*(tmp - 2.0);
               b = (tmp - 1.0)*(al*al - be*be + tmp*(tmp - 2.0)*x);
               c = 2.0*(ii - 1 + al)*(ii - 1 + be)*tmp;
               p0 = (b*p1 - c*p2)/a;
            }

            // Store some bits and pieces.
            this->_p1 = p1;
            this->_tmp = tmp;

            return p0;
         }

         value_type
         operator()( const value_type& x )
         {
            return jacobi::operator()( this->_order, x );
         }

         value_type
         derivative( unsigned order,
                     value_type x,
                     value_type value ) const
         {
            value_type al = this->_alpha, be = this->_beta, p1 = this->_p1, tmp = this->_tmp;
            return (order*(al - be - tmp*x)*value + 2.0*(order + al)*(order + be)*p1)/(tmp*(1.0 - x*x));
         }

         value_type
         derivative( value_type x,
                     value_type value ) const
         {
            return this->derivative( this->_order, x, value );
         }

         value_type
         previous_order() const
         {
            return this->_p1;
         }

      protected:

         unsigned _order;
         value_type _alpha;
         value_type _beta;
         value_type _p1;
         value_type _tmp;
      };

      template< class T >
      class jacobi_norm
         : public jacobi< T >
      {
      public:

         typedef T value_type;

         jacobi_norm( value_type alpha,
                      value_type beta )
            : jacobi<value_type>( alpha, beta )
         {
            this->_norm = this->_calc_norm( this->_order );
         }

         jacobi_norm( unsigned order,
                      value_type alpha,
                      value_type beta )
            : jacobi<value_type>( order, alpha, beta )
         {
            this->_norm = this->_calc_norm( order );
         }

         value_type
         operator()( unsigned order,
                     const value_type& x )
         {
            value_type p = jacobi<value_type>::operator()( order, x );
            value_type norm = this->_calc_norm( order );
            return norm*p;
         }

         value_type
         operator()( const value_type& x )
         {
            value_type p = jacobi<value_type>::operator()( x );
            return this->_norm*p;
         }

         value_type
         derivative( unsigned order,
                     value_type x,
                     value_type value ) const
         {
            return this->_calc_norm( order )*jacobi<T>::derivative( order, x, value );
         }

         value_type
         derivative( value_type x,
                     value_type value ) const
         {
            return this->_norm*jacobi<T>::derivative( this->_order, x, value );
         }

      protected:

         value_type
         _calc_norm( unsigned order )
         {
            return 1.0/sqrt( 2.0/(2.0*order + 1.0) );
         }

      private:

         value_type _norm;
      };

      template< class Jacobi >
      class jacobi_derivative
         : Jacobi
      {
      public:

         typedef typename Jacobi::value_type value_type;

         jacobi_derivative( value_type alpha,
                            value_type beta )
            : Jacobi( alpha + 1.0, beta + 1.0 )
         {
         }

         jacobi_derivative( unsigned order,
                            value_type alpha,
                            value_type beta )
            : Jacobi( order, alpha + 1.0, beta + 1.0 )
         {
         }

         value_type
         operator()( unsigned order,
                     const value_type& x )
         {
            if( order == 0 )
               return 0.0;
            else
               return sqrt( order*(order + 1.0) )*Jacobi::operator()( order - 1, x );
         }

         value_type
         operator()( const value_type& x )
         {
            return operator()( this->_order, x );
         }
      };

      template< class Polynomial1D >
      class tensor_product
      {
      public:

         typedef typename Polynomial1D::value_type value_type;

         tensor_product( unsigned dim,
                         unsigned order,
                         const Polynomial1D& poly )
            : _dim( dim ),
              _poly( poly )
         {
            this->_grid.setup_block( dim, order + 1 );
            this->_idxs.resize( dim );
         }

         value_type
         operator()( unsigned mode,
                     const typename vector<value_type>::view& x )
         {
            ASSERT( this->_dim > 0 );
            ASSERT( x.size() == this->_dim );

            this->_grid.lift( mode, this->_idxs.begin() );
            value_type result = this->_poly( this->_idxs[0], x[0] );
            for( unsigned ii = 1; ii < this->_dim; ++ii )
               result *= this->_poly( this->_idxs[ii], x[ii] );
            return result;
         }

      protected:

         unsigned _dim;
         Polynomial1D _poly;
         grid<unsigned> _grid;
         vector<unsigned> _idxs;
      };

      template< class Polynomial1D,
                class PolynomialDeriv1D >
      class tensor_product_derivative
         : public tensor_product< Polynomial1D >
      {
      public:

         typedef typename Polynomial1D::value_type value_type;

         tensor_product_derivative( unsigned dim,
                                    unsigned order,
                                    const Polynomial1D& poly,
                                    const PolynomialDeriv1D& poly_deriv )
            : tensor_product<Polynomial1D>( dim, order, poly ),
              _poly_deriv( poly_deriv )
         {
         }

         value_type
         operator()( unsigned mode,
                     unsigned deriv_dim,
                     const typename vector<value_type>::view& x )
         {
            ASSERT( this->_dim > 0 );
            ASSERT( x.size() == this->_dim );
            ASSERT( deriv_dim < this->_dim );

            this->_grid.lift( mode, this->_idxs.begin() );
            value_type result = 1.0;
            for( unsigned ii = 0; ii < this->_dim; ++ii ) {
               if( ii == deriv_dim )
                  result *= this->_poly_deriv( this->_idxs[ii], x[ii] );
               else
                  result *= this->_poly( this->_idxs[ii], x[ii] );
            }
            return result;
         }

      private:

         PolynomialDeriv1D _poly_deriv;
      };
   }
}

#endif
