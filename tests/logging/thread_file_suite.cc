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

#include <libhpc/debug/unit_test_main.hh>
#include <libhpc/logging/thread_file.hh>

using namespace hpc::test;

namespace {

   void
   do_something( hpc::logging::thread::file& log )
   {
      log << "no indent" << hpc::setindent( 2 ) << hpc::logging::endl;
      log << "indent" << hpc::setindent( -2 ) << hpc::logging::endl;
      log << "no indent again" << hpc::logging::endl;
   }

   test_case<> ANON(
      "/serial",
      "",
      []()
      {
         // hpc::logging::thread::file log( "test.log." );
         // do_something( log );
      }
      );

   test_case<> ANON(
      "/multithread",
      "",
      []()
      {
         // hpc::logging::thread::file log( "test.log." );
         // std::thread thread( std::bind( do_something, std::ref( log ) ) );
         // do_something( log );
         // thread.join();
      }
      );

}
