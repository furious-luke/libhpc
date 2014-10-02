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

#ifndef libhpc_numerics_interp_hh
#define libhpc_numerics_interp_hh

#include <vector>
#include "libhpc/debug/assert.hh"
#include "libhpc/system/assign.hh"

namespace hpc {
   namespace num {

      ///
      ///
      ///
      template< class T >
      class interp
      {
      public:

         typedef T value_type;

      public:

         template< class Seq >
         void
         set_abscissa( Seq&& abs )
         {
            hpc::assign( _abs, std::forward<Seq>( abs ) );
         }

         template< class Seq >
         void
         set_values( Seq&& vals )
         {
            hpc::assign( _vals, std::forward<Seq>( vals ) );
         }

         value_type
         operator[]( const value_type& x ) const
         {
            ASSERT( _abs.size() == _vals.size(), "Incompatible abscissa and values." );

            auto it = std::lower_bound( _abs.begin(), _abs.end(), x );
            unsigned low = std::distance( _abs.begin(), it );
            unsigned upp;
            if( low == 0 )
               upp = low + 1;
            else if( low >= _abs.size() )
            {
               low = _abs.size() - 2;
               upp = low + 1;
            }
            else
            {
               upp = low;
               low -= 1;
            }
            value_type fac = (x - _abs[low])/(_abs[upp] - _abs[low]);
            return _vals[low] + (_vals[upp] - _vals[low])*fac;
         }

      protected:

         std::vector<value_type> _abs;
         std::vector<value_type> _vals;
      };

   }
}

#endif
