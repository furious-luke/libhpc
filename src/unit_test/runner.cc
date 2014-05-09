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
#include "runner.hh"
#include "failures.hh"

namespace hpc {
   namespace test {

      extern test_case_node_t* head;

      void
      runner::run( test_case_base& tc )
      {
         std::cout << tc.name() << " " << std::flush;
         tc.run();
         tc.print_results();
         if( (bool)tc )
            std::cout << " ok\n" << std::flush;
         else
            std::cout << " FAILED\n" << std::flush;
      }

      void
      runner::run_all()
      {
         test_case_node_t* node = head;
         while( node )
         {
            node->tc->set_runner( this );
            run( *node->tc );
            node = node->next;
         }
      }

   }
}
