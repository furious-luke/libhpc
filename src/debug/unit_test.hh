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

#define TEST( expr, ... )                                               \
   (::hpc::test::decompose()->*expr).set_expr_str( #expr ).test( tc, ##__VA_ARGS__ )

#define ANON2( x, y )                           \
   x##y
#define ANON1( x, y )                           \
   ANON2( x, y )
#define ANON                                    \
   ANON1( tc_, __LINE__ )

namespace hpc {
   namespace test {

      template< class > class side;
      class test_case;
      template< class, class > class expression;

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

         test_expression_failed( const test_case& tc,
                                 const expression<T,U>& expr,
                                 const std::string& desc );

         virtual
         const char*
         what() const noexcept
         {
            return _msg.c_str();
         }

      protected:

         const test_case& _tc;
         const expression<T,U>& _expr;
         const std::string& _desc;
         std::string _msg;
      };

      class decompose
      {
      public:

         template< class T >
         side<T>
         operator->*( const T& op ) const
         {
            return side<T>( op );
         }
      };

      template< class, class > class expression;

      template< class T >
      class side
      {
      public:

         side( const T& val )
            : _val( val )
         {
         }

         template< class U >
         expression<T,U>
         operator==( const U& op ) const
         {
            return expression<T,U>( *this, side<U>( op ), expression<T,U>::EQUAL );
         }

         template< class U >
         expression<T,U>
         operator!=( const U& op ) const
         {
            return expression<T,U>( *this, side<U>( op ), expression<T,U>::NOT_EQUAL );
         }

         const T&
         operator*() const
         {
            return _val;
         }

      protected:

         const T& _val;
      };

      class test_case;

      template< class T,
                class U >
      class expression
      {
      public:

         enum expression_type
         {
            EQUAL,
            NOT_EQUAL
         };

      public:

         expression( const side<T>& left,
                     const side<U>& right,
                     expression_type op )
            : _left( left ),
              _right( right ),
              _op( op )
         {
         }

         expression&
         set_expr_str( const char* expr_str )
         {
            _expr_str = expr_str;
            return *this;
         }

         operator bool() const
         {
            switch( _op )
            {
               case EQUAL:
                  return *_left == *_right;
                  break;

               case NOT_EQUAL:
                  return *_left != *_right;
                  break;

               default:
                  assert( 0 );
            }
         }

         void
         test( test_case& tc,
               const std::string& desc = std::string() );

         const T&
         lhs() const
         {
            return *_left;
         }

         const U&
         rhs() const
         {
            return *_right;
         }

         const char*
         str() const
         {
            return _expr_str;
         }

      protected:

         std::string
         _op_str() const
         {
            switch( _op )
            {
               case EQUAL:
                  return "==";
               case NOT_EQUAL:
                  return "!=";
            };
            assert( 0 );
         }

      protected:

         side<T> _left;
         side<U> _right;
         expression_type _op;
         const char* _expr_str;
      };

      class test_case
      {
      public:

         test_case( const std::string& name,
                    const std::string& desc,
                    std::function<void(test_case&)> func );

         void
         run();

         template< class T,
                   class U >
         void
         test_expression( const expression<T,U>& expr,
                          const std::string& desc )
         {
            if( !expr )
            {
               // Failed, log the failure and conclude this test.
               throw test_expression_failed<T,U>( *this, expr, desc );
            }
            else
               std::cout << ".";
         }

         const std::string&
         name() const
         {
            return _name;
         }

      protected:

         void
         _add_test_case();

      protected:

         std::string _name;
         std::string _desc;
         std::function<void(test_case&)> _func;
      };

      template< class Fixture >
      class test_case_fix
         : public test_case
      {
      protected:

         Fixture fix;
      };

      class runner
      {
      public:

         virtual void
         run( test_case& tc );

         void
         run_all();
      };

      template< class T,
                class U >
      test_expression_failed<T,U>::test_expression_failed( const test_case& tc,
                                                           const expression<T,U>& expr,
                                                           const std::string& desc )
         : _tc( tc ),
           _expr( expr ),
           _desc( desc )
      {
         boost::format fmt( "\n  Failed as a result of expression:\n"
                            "    %2%\n"
                            "  where\n"
                            "    LHS: %3%\n"
                            "    RHS: %4%\n" );
         fmt % tc.name();
         fmt % expr.str() % expr.lhs() % expr.rhs();
         _msg = fmt.str();
      }

      template< class T,
                class U >
      void
      expression<T,U>::test( test_case& tc,
                             const std::string& desc )
      {
         tc.test_expression( *this, desc );
      }

   }
}

#endif
