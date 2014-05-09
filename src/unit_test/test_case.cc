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

#include <iostream>
#include "test_case.hh"

namespace hpc {
   namespace test {

      test_case_node_t* head = NULL;

      test_case_base::test_case_base( std::string const& name,
                                      std::string const& desc )
         : _name( name ),
           _desc( desc ),
           _runner( 0 ),
           _succ( false )
      {
         _add_test_case();
      }

      std::string const&
      test_case_base::name() const
      {
         return _name;
      }

      void
      test_case_base::set_runner( test::runner const* runner )
      {
         _runner = runner;
      }

      test::runner const*
      test_case_base::runner() const
      {
         return _runner;
      }

      void
      test_case_base::_add_test_case()
      {
         if( head )
         {
            test_case_node_t* node = head;
            while( node->next )
               node = node->next;
            node->next = (test_case_node_t*)malloc( sizeof(test_case_node_t) );
            node->next->next = NULL;
            node->next->tc = this;
         }
         else
         {
            head = (test_case_node_t*)malloc( sizeof(test_case_node_t) );
            head->next = NULL;
            head->tc = this;
         }
      }

      test_case::test_case( const std::string& name,
                            const std::string& desc,
                            boost::function<void(result_buffer<>&)> func )
         : test_case_base( name, desc ),
           _func( func )
      {
         _buf.resize( 100000 );
         _rb.set_buffer( _buf.data() );
      }

      void
      test_case::run()
      {
         _func( _rb );

         // Search for any failures.
         std::vector<char>::const_iterator it = _buf.begin();
         _succ = true;
         while( it != _buf.end() && *it )
         {
            bool succ = *it++ == 1;
            _succ = succ && _succ;
         }
      }

      void
      test_case::print_results() const
      {
         std::vector<char>::const_iterator it = _buf.begin();
         while( it != _buf.end() && *it )
         {
            bool succ = *it++ == 1;
            std::cout << (succ ? "." : "X");
         }
      }

   }
}
