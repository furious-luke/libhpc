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

#ifndef hpc_unit_test_failures_hh
#define hpc_unit_test_failures_hh

#include <boost/format.hpp>
#include "libhpc/system/stream.hh"
#include "libhpc/system/string.hh"
#include "runner.hh"

namespace hpc {
   namespace test {

      template< class, class >
      class expression;

      class test_failed
         : public std::exception
      {
      };

      template< class T,
                class U >
      class test_expression_failed
         : public test_failed
      {
      public:

         test_expression_failed( expression<T,U> const* expr,
                                 std::string const& desc = std::string() )
            : _expr( expr ),
              _desc( desc )
         {
            boost::format fmt( "\n  Failed as a result of expression:\n"
                               "    %1%\n"
                               "  where\n"
                               "    LHS: %2%\n"
                               "    RHS: %3%\n"
                               "  at\n"
                               "    %4%:%5%\n" );
            if( expr->str() )
               fmt % expr->str();
            else
               fmt % (std::string( expr->lhs_str() ) + " == " + std::string( expr->rhs_str() ) +
                      " within tolerance of " + to_string( expr->epsilon() ));
            fmt % expr->lhs() % expr->rhs();
            fmt % expr->file() % expr->line();
            _msg = fmt.str();
         }

         ~test_expression_failed() throw()
         {
         }

         virtual
         char const*
         what() const throw()
         {
            return _msg.c_str();
         }

      protected:

         expression<T,U> const* _expr;
         std::string const& _desc;
         std::string _msg;
      };

   }
}

#endif
