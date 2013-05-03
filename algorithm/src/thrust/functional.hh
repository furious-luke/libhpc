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

#ifndef hpc_algorithm_thrust_functional_hh
#define hpc_algorithm_thrust_functional_hh

#include <functional>
#include "libhpc/algorithm/cuda_helpers.hh"

namespace hpc {
   namespace thrust {

      ///
      ///
      ///
      template< typename Operation >
      class binder1st
         : public std::unary_function< typename Operation::second_argument_type,
                                       typename Operation::result_type >
      {
      public:

         typedef Operation operation_type;
         typedef typename operation_type::result_type result_type;
         typedef typename operation_type::first_argument_type first_argument_type;
         typedef typename operation_type::second_argument_type second_argument_type;

      public:

         DECLARE_DEVICE_HOST
         binder1st( const Operation& op,
                    const first_argument_type& x )
            : _op( op ),
              _x( x )
         {
         }

         DECLARE_DEVICE_HOST
         result_type
         operator()( const second_argument_type& y ) const
         {
            return _op( _x, y );
         }

      protected:

         operation_type _op;
         first_argument_type _x;
      };

      ///
      ///
      ///
      template< typename Operation,
                typename T >
      inline
      binder1st<Operation>
      bind1st( const Operation& op,
               const T& x )
      {
         typedef typename Operation::first_argument_type first_argument_type;
         return binder1st<Operation>( op, first_argument_type( x ) );
      }

      ///
      ///
      ///
      template< typename Operation >
      class binder2nd
         : public std::unary_function< typename Operation::first_argument_type,
                                       typename Operation::result_type >
      {
      public:

         typedef Operation operation_type;
         typedef typename operation_type::result_type result_type;
         typedef typename operation_type::first_argument_type first_argument_type;
         typedef typename operation_type::second_argument_type second_argument_type;

      public:

         DECLARE_DEVICE_HOST
         binder2nd( const Operation& op,
                    const second_argument_type& y )
            : _op( op ),
              _y( y )
         {
         }

         DECLARE_DEVICE_HOST
         result_type
         operator()( const first_argument_type& x ) const
         {
            return _op( x, _y );
         }

      protected:

         operation_type _op;
         second_argument_type _y;
      };

      ///
      ///
      ///
      template< typename Operation,
                typename T >
      inline
      binder2nd<Operation>
      bind2nd( const Operation& op,
               const T& y )
      {
         typedef typename Operation::second_argument_type second_argument_type;
         return binder2nd<Operation>( op, second_argument_type( y ) );
      }

   }
}

#endif
