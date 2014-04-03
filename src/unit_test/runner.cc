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

namespace hpc {
   namespace test {

      void
      runner::run( test_case_base& tc )
      {
         std::cout << tc.name() << " " << std::flush;
         try
         {
            tc.run();
            std::cout << " ok\n";
         }
         catch( test_failed& ex )
         {
            std::cout << ex.what();
         }
      }

      void
      runner::run_all()
      {
         test_case_node_t* node = head;
         while( node )
         {
            run( *node->tc );
            node = node->next;
         }
      }

   }
}
