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
#include <libhpc/h5/derive.hh>

struct test_struct
{
   int an_int;
   char a_byte;
   float a_float;
};

TEST_CASE( "/libhpc/h5/derive" )
{
   hpc::h5::derive der( sizeof(test_struct) );
   der.add( hpc::h5::datatype::native_int,
            HOFFSET( test_struct, an_int ),
            hpc::h5::datatype::std_i32be,
            "An integer!" );
   der.add( hpc::h5::datatype::native_char,
            HOFFSET( test_struct, a_byte ),
            hpc::h5::datatype::native_char,
            "A byte!" );
   der.add( hpc::h5::datatype::native_float,
            HOFFSET( test_struct, a_float ),
            hpc::h5::datatype::native_float,
            "A float!" );
   hpc::h5::datatype mem_type, file_type;
   der.commit( mem_type, file_type );
   // TEST( mem_type.size() == 9 );
   TEST( file_type.size() == 9 );
}
