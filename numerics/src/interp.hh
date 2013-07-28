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

#include "libhpc/debug/debug.hh"
#include "libhpc/containers/vector.hh"

class interp_suite;

namespace hpc {
   namespace numerics {

      ///
      ///
      ///
      template< class T >
      class interp
      {
         friend class ::interp_suite;

      public:

         typedef T value_type;

      public:

         void
         set_abscissa( hpc::vector<value_type>& abs )
         {
            _abs.take( abs );
         }

         void
         set_values( hpc::vector<value_type>& vals )
         {
            _vals.take( vals );
         }

         value_type
         operator[]( const value_type& x )
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

         hpc::vector<value_type> _abs;
         hpc::vector<value_type> _vals;
      };

   }
}

#endif
