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

#ifndef libhpc_unit_test_expression_hh
#define libhpc_unit_test_expression_hh

namespace hpc {
   namespace test {

      template< class T >
      class side
      {
      public:

         side( T const& val )
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
         operator<=( const U& op ) const
         {
            bool res = _val <= op;
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
         operator>=( const U& op ) const
         {
            bool res = _val >= op;
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

         T const& _val;
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

      class decompose
      {
      public:

         template< class T >
         side<T>
         operator->*( T const& op ) const
         {
            return side<T>( op );
         }
      };

   }
}

#endif
