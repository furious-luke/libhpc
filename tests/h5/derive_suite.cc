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
#include "libhpc/h5/derive.hh"

using namespace hpc;
using namespace hpc::test;

namespace {

   struct test_struct
   {
      int an_int;
      byte a_byte;
      float a_float;
   };

   test_case<> ANON(
      "/h5/derive",
      "",
      []()
      {
	 h5::derive der;
	 der.add( h5::datatype::native_int, HOFFSET( test_struct, an_int ), h5::datatype::std_i32be, "An integer!" );
	 der.add( h5::datatype::native_char, HOFFSET( test_struct, a_byte ), h5::datatype::native_char, "A byte!" );
	 der.add( h5::datatype::native_float, HOFFSET( test_struct, a_float ), h5::datatype::native_float, "A float!" );
	 h5::datatype mem_type, file_type;
	 der.commit( mem_type, file_type );
	 // TEST( mem_type.size() == 9 );
	 TEST( file_type.size() == 9 );
      }
      );

}
