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

#define ANON2( x, y )                           \
   x##y
#define ANON1( x, y )                           \
   ANON2( x, y )
#define ANON                                    \
   ANON1( hpc_test_case_, __COUNTER__ )

namespace hpc {
   namespace test {

      enum comparison_type
      {
         EQUAL,
         NOT_EQUAL
      };

      template< class > class side;
      class test_case_base;
      template< class, class > class expression;

      extern test_case_base* _cur_tc;

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

         test_expression_failed( const test_case_base& tc,
                                 const expression<T,U>& expr,
                                 const std::string& desc );

         virtual
         const char*
         what() const noexcept
         {
            return _msg.c_str();
         }

      protected:

         const test_case_base& _tc;
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
            bool res = _val == op;
            return expression<T,U>( *this, side<U>( op ), res );
         }

         template< class U >
         expression<T,U>
         operator!=( const U& op ) const
         {
            bool res = _val != op;
            return expression<T,U>( *this, side<U>( op ), res );
         }

         template< class U >
         expression<T,U>
         operator<( const U& op ) const
         {
            bool res = _val < op;
            return expression<T,U>( *this, side<U>( op ), res );
         }

         template< class U >
         expression<T,U>
         operator>( const U& op ) const
         {
            bool res = _val > op;
            return expression<T,U>( *this, side<U>( op ), res );
         }

         template< class U >
         expression<T,U>
         delta( const U& op,
                double epsilon ) const
         {
            bool res = (_val >= op - epsilon && _val <= op + epsilon);
            return expression<T,U>( *this, side<U>( op ), res );
         }

         const T&
         operator*() const
         {
            return _val;
         }

      protected:

         const T& _val;
      };

      template< class T,
                class U >
      class expression
      {
      public:

         expression( const side<T>& left,
                     const side<U>& right,
                     bool result )
            : _left( left ),
              _right( right ),
              _res( result )
         {
         }

         expression&
         set_info( const char* expr_str,
                   const char* file,
                   int line )
         {
            _expr_str = expr_str;
            _file = file;
            _line = line;
            return *this;
         }

         expression&
         set_delta_info( const char* lhs,
                         const char* rhs,
                         double epsilon,
                         const char* file,
                         int line )
         {
            _expr_str = boost::str( boost::format( "%1% == %2% within tolerance of %3%" ) % lhs % rhs % epsilon );
            _file = file;
            _line = line;
            return *this;
         }

         void
         test( test_case_base& tc,
               const std::string& desc = std::string() );

         operator bool()
         {
            return _res;
         }

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

         const std::string&
         str() const
         {
            return _expr_str;
         }

         const char*
         file() const
         {
            return _file;
         }

         int
         line() const
         {
            return _line;
         }

      protected:

         side<T> _left;
         side<U> _right;
         bool _res;
         std::string _expr_str;
         const char* _file;
         int _line;
      };

      class test_case_base
      {
      public:

         test_case_base( const std::string& name,
                         const std::string& desc );

         virtual
         void
         run() = 0;

         const std::string&
         name() const;

      protected:

         void
         _add_test_case();

      protected:

         std::string _name;
         std::string _desc;
      };

      template< class Fixture = void >
      class test_case
         : public test_case_base
      {
      public:

         test_case( const std::string& name,
                    const std::string& desc,
                    std::function<void(Fixture&)> func )
            : test_case_base( name, desc ),
              _func( func ),
              _fix( 0 )
         {
         }

         virtual
         ~test_case()
         {
            if( _fix )
            {
               delete _fix;
               _fix = 0;
            }
         }

         virtual
         void
         run()
         {
            _cur_tc = this;
            _fix = new Fixture;
            _func( *_fix );
            delete _fix;
            _fix = 0;
         }

      protected:

         std::function<void(Fixture&)> _func;
         Fixture* _fix;
      };

      template<>
      class test_case<void>
         : public test_case_base
      {
      public:

         test_case( const std::string& name,
                    const std::string& desc,
                    std::function<void()> func )
            : test_case_base( name, desc ),
              _func( func )
         {
         }

         virtual
         void
         run()
         {
            _cur_tc = this;
            _func();
         }

      protected:

         std::function<void()> _func;
      };

      class runner
      {
      public:

         virtual void
         run( test_case_base& tc );

         void
         run_all();
      };

      template< class T,
                class U >
      test_expression_failed<T,U>::test_expression_failed( const test_case_base& tc,
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
                            "    RHS: %4%\n"
                            "  at\n"
                            "    %5%:%6%\n" );
         fmt % tc.name();
         fmt % expr.str() % expr.lhs() % expr.rhs();
         fmt % expr.file() % expr.line();
         _msg = fmt.str();
      }

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
            std::cout << ".";
      }

   }
}

#endif
