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
      test_case_base* _cur_tc;

      test_case_base::test_case_base( std::string const& name,
                                      std::string const& desc )
         : _name( name ),
           _desc( desc )
      {
         _add_test_case();
      }

      std::string const&
      test_case_base::name() const
      {
         return _name;
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

   }
}
