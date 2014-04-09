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

#include <libhpc/unit_test/main.hh>
#include <libhpc/logging/thread_file.hh>

void
do_something( hpc::log::thread::file& log )
{
   log << "no indent" << hpc::setindent( 2 ) << hpc::log::endl;
   log << "indent" << hpc::setindent( -2 ) << hpc::log::endl;
   log << "no indent again" << hpc::log::endl;
}

TEST_CASE( "/libhpc/logging/thread_file/serial" )
{
   // hpc::logging::thread::file log( "test.log." );
   // do_something( log );
}

TEST_CASE( "/libhpc/logging/thread_file/multithreaded" )
{
   // hpc::logging::thread::file log( "test.log." );
   // std::thread thread( std::bind( do_something, std::ref( log ) ) );
   // do_something( log );
   // thread.join();
}
