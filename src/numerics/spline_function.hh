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

#ifndef libhpc_numerics_spline_function_hh
#define libhpc_numerics_spline_function_hh

namespace hpc {
   namespace numerics {

      template< class Spline,
		class Func >
      class spline_function
      {
      public:

         typedef Spline spline_type;
         typedef Func function_type;
         typedef typename function_type::value_type value_type;

      public:

         spline_function( const spline_type& sp,
                          Func func )
            : _sp( sp ),
              _func( func )
         {
         }

         value_type
         operator()( value_type x,
                     unsigned sp_poly )
         {
            return _func( x, _sp( x, sp_poly ) );
         }

         const spline_type&
         spline() const
         {
            return _sp;
         }

      protected:

         const spline_type& _sp;
         Func _func;
      };

   }
}

#endif
