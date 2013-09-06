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

#ifndef libhpc_numerics_spline_spline_function_hh
#define libhpc_numerics_spline_spline_function_hh

namespace hpc {
   namespace numerics {

      template< class Spline0,
		class Spline1,
		class Func >
      class spline_spline_function
      {
      public:

         typedef Spline0 spline0_type;
         typedef Spline1 spline1_type;
         typedef Func function_type;
         typedef typename function_type::value_type value_type;

      public:

         spline_spline_function( const spline0_type& sp0,
                                 const spline1_type& sp1,
                                 Func func )
            : _sp0( sp0 ),
              _sp1( sp1 ),
              _func( func )
         {
         }

         value_type
         operator()( value_type x,
                     unsigned sp0_poly,
                     unsigned sp1_poly )
         {
            return _func( x, _sp0( x, sp0_poly ), _sp1( x, sp1_poly ) );
         }

         const spline0_type&
         spline0() const
         {
            return _sp0;
         }

         const spline1_type&
         spline1() const
         {
            return _sp1;
         }

      protected:

         const spline0_type& _sp0;
         const spline0_type& _sp1;
         Func _func;
      };

   }
}

#endif
