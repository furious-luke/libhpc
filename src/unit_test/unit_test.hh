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

#ifndef libhpc_debug_unit_test_hh
#define libhpc_debug_unit_test_hh

#include <iostream>
#include <string>
#include <functional>
#include <assert.h>
#include <boost/format.hpp>
#include "libhpc/system/anon.hh"

#define TEST( expr, ... )                       \
   (::hpc::test::decompose()->*expr).set_info(  \
      #expr, __FILE__, __LINE__ ).test(         \
         *::hpc::test::_cur_tc, ##__VA_ARGS__ )

#define DELTA( lhs, rhs, epsilon, ... )                 \
   (::hpc::test::decompose()->*lhs)                     \
   .delta( rhs, epsilon )                               \
   .set_delta_info(                                     \
      #lhs, #rhs, epsilon, __FILE__, __LINE__ ).test(   \
         *::hpc::test::_cur_tc, ##__VA_ARGS__ )

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

namespace hpc {
   namespace test {



      template< class T,
                class U >
      void
      expression<T,U>::test( test_case_base& tc,
                             const std::string& desc )
      {
         if( !(*this) )
         {
            // Failed, log the failure and conclude this test.
            throw test_expression_failed<T,U>( tc, *this, desc );
         }
         else
            std::cout << "." << std::flush;
      }

   }
}

#endif
