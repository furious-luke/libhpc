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

#ifndef libhpc_unit_test_failures_hh
#define libhpc_unit_test_failures_hh

#include <boost/format.hpp>
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

         test_expression_failed( test_case_base const& tc,
                                 expression<T,U> const& expr,
                                 std::string const& desc )
            : _tc( tc ),
              _expr( expr ),
              _desc( desc )
         {
            boost::format fmt( "\n  Failed as a result of expression:\n"
                               "    %2%\n"
                               "  where\n"
                               "    LHS: %3%\n"
                               "    RHS: %4%\n"
                               "  at\n"
                               "    %5%:%6%\n" );
            fmt % tc.name();
            fmt % expr.str() % expr.lhs() % expr.rhs();
            fmt % expr.file() % expr.line();
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

         test_case_base const& _tc;
         expression<T,U> const& _expr;
         std::string const& _desc;
         std::string _msg;
      };

   }
}

#endif
