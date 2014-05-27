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

#ifndef hpc_unit_test_unit_test_hh
#define hpc_unit_test_unit_test_hh

#include <iostream>
#include <string>
#include <functional>
#include <assert.h>
#include <boost/format.hpp>
#include "libhpc/system/anon.hh"
#include "expression.hh"

#ifndef __CUDA_ARCH__

#define TEST( expr, ... )                       \
   (::hpc::test::decompose()->*expr)            \
   .set_info( #expr, __FILE__, __LINE__ )       \
   .test( rb, ##__VA_ARGS__ )

#define TEST_EQ( lhs, rhs )                     \
   TEST( (lhs) == (rhs) )

#define DELTA( lhs, rhs, eps, ... )                             \
   (::hpc::test::decompose()->*lhs)                             \
   .delta( rhs, eps )                                           \
   .set_delta_info( #lhs, #rhs, eps, __FILE__, __LINE__ )       \
   .test( rb, ##__VA_ARGS__ )

#define THROWS( expr, ex, ... )                         \
   try {                                                \
      expr;                                             \
      throw hpc::test::test_failed();                   \
   } catch( ex& e ) {                                   \
      std::cout << "." << std::flush;                   \
   } catch( ... ) {                                     \
      throw hpc::test::test_failed();                   \
   }

#define THROWS_ANY( expr, ... )                         \
   try {                                                \
      expr;                                             \
      throw hpc::test::test_failed();                   \
   } catch( ... ) {                                     \
      std::cout << "." << std::flush;                   \
   }

#define THROWS_NONE( expr, ... )                        \
   try {                                                \
      expr;                                             \
      std::cout << "." << std::flush;                   \
   } catch( ... ) {                                     \
      throw hpc::test::test_failed();                   \
   }

#else // __CUDA_ARCH__

template< class T,
          class U >
CUDA_DEV_HOST
bool
__hpc_test_delta( T const& lhs,
		  U const& rhs,
		  double eps )
{
   return (lhs >= rhs - eps && rhs <= rhs + eps);
}

#define TEST_EQ( lhs, rhs )                     \
   rb.push( (lhs) == (rhs) )

#define DELTA( lhs, rhs, eps, ... )		\
   rb.push( __hpc_test_delta( lhs, rhs, eps ) )

#endif // __CUDA_ARCH__

#endif
