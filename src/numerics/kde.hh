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
#include "libhpc/containers/view.hh"

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

         template< class T >
         class constant
         {
         public:

            typedef T value_type;

         public:

            constant( value_type h )
               : _h( h ),
                 _h_inv( 1.0/h )
            {
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

         kde()
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

      protected:

         std::vector<value_type> _pnts;
         bandwidth_type _bw;
         kernel_type _kern;
      };

      template< class T,
                class Bandwidth,
                class Kernel,
                class Enable = boost::enable_if<boost::is_derived<Bandwidth,bandwidths::constant>>::type >
      class kde
         : public kde_base
      {
      public:

         typedef T value_type;
         typedef Bandwidth bandwidth_type;
         typedef Kernel kernel_type;

      public:

         kde()
         {
         }

         template< class Seq >
         kde( Seq&& pnts,
              value_type h )
            : _pnts( std::forward<Seq>( pnts ) ),
              _bw( h )
         {
         }

         value_type
         eval( value_type x )
         {
            return sum( x )*_bw.h_inv()/(double)_pnts.size();
         }

      protected:

         std::vector<value_type> _pnts;
         bandwidth_type _bw;
         kernel_type _kern;
      };

   }
}

#endif
