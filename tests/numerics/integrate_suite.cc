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

#include <libhpc/unit_test/main.hh>
#include <libhpc/numerics/integrate.hh>
#include <libhpc/numerics/quadrature.hh>

using namespace hpc::num;

template< class T >
struct constant
{
   typedef T result_type;

   constant( T val )
   : value( val )
   {
   }

   template< class Point >
   result_type
   operator()( const Point& pnt ) const
   {
      return value;
   }

   T value;
};

template< class T >
struct linear
{
   typedef T result_type;

   linear( T grad,
           T cut )
      : gradient( grad ),
        cut( cut )
   {
   }

   template< class Point >
   result_type
   operator()( const Point& pnt ) const
   {
      return pnt*gradient + cut;
   }

   T gradient;
   T cut;
};

template< class T >
struct quadratic
{
   typedef T result_type;

   template< class Point >
   result_type
   operator()( const Point& pnt ) const
   {
      return pnt*pnt;
   }

   T value;
};

TEST_CASE( "/hpc/numerics/integrate/quadrature_summation/1d" )
{
   // Constant integration of 1 over interval of 2.
   {
      unsigned order = 0;
      unsigned size = order + 1;
      std::vector<double> points( size ), weights( size );
      make_quadrature( gauss_legendre_generator<double>( -1.0, 1.0 ), order, points.begin(), weights.begin() );
      double sum = quadrature_summation( points.begin(), points.end(), weights.begin(), constant<double>( 1.0 ) );
      DELTA( sum, 2.0, 1e-6 );
   }

   // Linear over interval of 2.
   {
      unsigned order = 1;
      unsigned size = order + 1;
      std::vector<double> points( size ), weights( size );
      make_quadrature( gauss_legendre_generator<double>( -1.0, 1.0 ), order, points.begin(), weights.begin() );
      double sum = quadrature_summation( points.begin(), points.end(), weights.begin(), linear<double>( 1.0, 0.0 ) );
      DELTA( sum, 0.0, 1e-6 );
      sum = quadrature_summation( points.begin(), points.end(), weights.begin(), linear<double>( 1.0, 1.0 ) );
      DELTA( sum, 2.0, 1e-6 );
   }

   // Quadratic over interval of 2.
   {
      unsigned order = 2;
      unsigned size = order + 1;
      std::vector<double> points( size ), weights( size );
      make_quadrature( gauss_legendre_generator<double>( -1.0, 1.0 ), order, points.begin(), weights.begin() );
      double sum = quadrature_summation( points.begin(), points.end(), weights.begin(), quadratic<double>() );
      DELTA( sum, 2.0/3.0, 1e-6 );
   }
}
