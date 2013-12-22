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
#include <libhpc/system/path_finder.hh>

using namespace hpc::test;

namespace {

   test_case<> ANON(
      "/libhpc/system/path_finder/add_root",
      "",
      []()
      {
	 hpc::path_finder pf;
	 pf.add_root( "." );
	 pf.add_root( "/tmp" );
	 TEST( pf.roots().size() == 2 );
	 TEST( pf.roots().front() == "." );
	 TEST( pf.roots().back() == "/tmp" );
      }
      );

   test_case<> ANON(
      "/libhpc/system/path_finder/find",
      "",
      []()
      {
	 hpc::path_finder pf;
	 pf.add_root( "." );
	 pf.add_root( "/tmp" );
	 TEST( *pf.find( "auth/login.html" ) == "./auth/login.html" );
	 TEST( pf.find( "nothing" ) == boost::none );
      }
      );

}
