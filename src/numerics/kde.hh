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

#ifndef libhpc_numerics_kde_hh
#define libhpc_numerics_kde_hh

#include <vector>
#include <array>
#include <memory>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include "libhpc/containers/view.hh"
#include "libhpc/containers/assign.hh"
#include "iqr.hh"

namespace hpc {
   namespace numerics {
      namespace kernels {

         template< class T >
         class epanechnikov
         {
         public:

            typedef T value_type;

         public:

            value_type
            operator()( value_type x )
            {
               return (x >= -1.0 && x <= 1.0) ? (0.75*(1 - x*x)) : 0.0;
            }
         };

      }

      namespace bandwidths {

         struct constant_tag {};

         template< class T >
         class constant
         {
         public:

            typedef T value_type;
            typedef constant_tag tag_type;

         public:

            constant( value_type h = 1 )
               : _h( h ),
                 _h_inv( 1.0/h )
            {
            }

            void
            set_h( value_type h )
            {
               _h = h;
               _h_inv = 1.0/h;
            }

            value_type
            h() const
            {
               return _h;
            }

            value_type
            h_inv() const
            {
               return _h_inv;
            }

            template< class Iter >
            void
            set_iqr( Iter const& start,
                     unsigned size )
            {
               _h = 0.79*interquartile_range( start, size )*pow( (double)size, -1.0/5.0 );
               _h_inv = 1.0/_h;
            }

            template< class Seq >
            void
            set_iqr( Seq const& seq )
            {
               set_iqr( seq.begin(), seq.size() );
            }

         public:

            value_type _h;
            value_type _h_inv;
         };

      }

      template< class T,
                class Bandwidth,
                class Kernel >
      class kde_base
      {
      public:

         typedef T value_type;
         typedef Bandwidth bandwidth_type;
         typedef Kernel kernel_type;

      public:

         kde_base( unsigned size = 0 )
            : _size( size )
         {
         }

         value_type
         sum( value_type x )
         {
            value_type y = 0.0;
            value_type h_inv = _bw.h_inv();
            for( auto p : _pnts )
               y += _kern( h_inv*(p - x) );
            return y;
         }

         typename hpc::view<std::vector<value_type>>::type const
         points() const
         {
            return _pnts;
         }

         bandwidth_type&
         bandwidth()
         {
            return _bw;
         }

      protected:

         std::vector<value_type> _pnts;
         unsigned _size;
         bandwidth_type _bw;
         kernel_type _kern;
      };

      template< class T,
                class Bandwidth = bandwidths::constant<T>,
                class Kernel = kernels::epanechnikov<T>,
                class Enable = boost::enable_if<
                   boost::is_same<typename Bandwidth::tag_type,bandwidths::constant_tag>
                   > >
      class kde
         : public kde_base<T,Bandwidth,Kernel>
      {
      public:

         typedef T value_type;
         typedef Bandwidth bandwidth_type;
         typedef Kernel kernel_type;
         typedef kde_base<T,Bandwidth,Kernel> super_type;

      public:

         kde()
         {
         }

         template< class Seq >
         kde( Seq&& pnts,
              unsigned size = 0 )
            : super_type( size )
         {
            hpc::assign( this->_pnts, std::forward<Seq>( pnts ) );
            this->_bw.set_iqr( this->_pnts );
         }

         value_type
         eval( value_type x )
         {
            double size = this->_size ? (double)this->_size : (double)this->_pnts.size();
            return this->sum( x )*this->_bw.h_inv()/size;
         }
      };

   }
}

#endif
