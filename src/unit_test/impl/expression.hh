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

#ifndef libhpc_unit_test_impl_expression_hh
#define libhpc_unit_test_impl_expression_hh

#include <iostream>
#include <string>

namespace hpc {
   namespace test {

      template< class T,
                class U >
      void
      expression<T,U>::test( test_case_base& tc,
                             std::string const& desc )
      {
         if( !(*this) )
         {
            // Failed, log the failure and conclude this test.
            throw test_expression_failed<T,U>( tc, *this, desc );
         }
         else
            tc.runner()->dot();
      }

   }
}

#endif
