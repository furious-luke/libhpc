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
#include <libhpc/system/array.hh>

template< class T >
hpc::array<T,2>
return_value()
{
   return hpc::array<T,2>{ 2, 3 };
}

TEST_CASE_CUDA( "/hpc/system/array/initialise" )
{
   hpc::array<int,2> ar{ 2, 3 };
   TEST_EQ( ar[0], 2 );
   TEST_EQ( ar[1], 3 );
}

TEST_CASE_CUDA( "/hpc/system/array/return" )
{
   hpc::array<int,2> ar = return_value<int>();
   TEST_EQ( ar[0], 2 );
   TEST_EQ( ar[1], 3 );
}
