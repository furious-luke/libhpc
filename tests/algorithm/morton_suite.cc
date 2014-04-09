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
#include <libhpc/algorithm/morton.hh>

TEST_CASE( "/libhpc/algorithm/morton/dilate/2d" )
{
   uint16_t inputs[] = { 0b0000000000000000, 0b0000000000000001, 0b0101010101010101, 0b1010101010101010, 0b1111111111111111 };
   uint32_t outputs[] = { 0b00000000000000000000000000000000, 0b00000000000000000000000000000001,
                          0b00010001000100010001000100010001, 0b01000100010001000100010001000100,
                          0b01010101010101010101010101010101 };
   unsigned size = sizeof(inputs)/sizeof(inputs[0]);
   for( unsigned ii = 0; ii < size; ++ii )
   {
      TEST( hpc::dilate<2>( inputs[ii] ) == outputs[ii] );
      TEST( hpc::undilate<2>( outputs[ii] ) == inputs[ii] );
   }
}

TEST_CASE( "/libhpc/algorithm/morton/dilate/3d" )
{
   uint16_t inputs[] = { 0b0000000000000000, 0b0000000000000001, 0b0000000101010101, 0b0000000010101010, 0b0000001111111111 };
   uint32_t outputs[] = { 0b00000000000000000000000000000000, 0b00000000000000000000000000000001,
                          0b00000001000001000001000001000001, 0b00000000001000001000001000001000,
                          0b00001001001001001001001001001001 };
   unsigned size = sizeof(inputs)/sizeof(inputs[0]);
   for( unsigned ii = 0; ii < size; ++ii )
   {
      TEST( hpc::dilate<3>( inputs[ii] ) == outputs[ii] );
      TEST( hpc::undilate<3>( outputs[ii] ) == inputs[ii] );
   }
}

TEST_CASE( "/libhpc/algorithm/morton/2d" )
{
   uint16_t inputs_x[] = { 0b0000000000000000, 0b0000000000000001, 0b0101010101010101, 0b1010101010101010, 0b1111111111111111,
                           0b0010100011010011 };
   uint16_t inputs_y[] = { 0b0000000000000000, 0b0000000000000001, 0b0101010101010101, 0b1010101010101010, 0b1111111111111111,
                           0b0111010100110100 };
   uint32_t outputs[] = { 0b00000000000000000000000000000000, 0b00000000000000000000000000000011,
                          0b00110011001100110011001100110011, 0b11001100110011001100110011001100,
                          0b11111111111111111111111111111111, 0b00101110011000100101101100100101 };
   unsigned size = sizeof(outputs)/sizeof(outputs[0]);
   for( unsigned ii = 0; ii < size; ++ii )
   {
      TEST( hpc::morton<2>( inputs_x[ii], inputs_y[ii] ) == outputs[ii] );
      TEST( hpc::unmorton<2>( outputs[ii] )[0] == inputs_x[ii] );
      TEST( hpc::unmorton<2>( outputs[ii] )[1] == inputs_y[ii] );
   }
}

TEST_CASE( "/libhpc/algorithm/morton/3d" )
{
   uint16_t inputs_x[] = { 0b0000000000001001 };
   uint16_t inputs_y[] = { 0b0000000000000101 };
   uint16_t inputs_z[] = { 0b0000000000001001 };
   uint32_t outputs[] = { 0b00000000000000000000101010000111 };
   unsigned size = sizeof(outputs)/sizeof(outputs[0]);
   for( unsigned ii = 0; ii < size; ++ii )
   {
      TEST( hpc::morton<3>( inputs_x[ii], inputs_y[ii], inputs_z[ii] ) == outputs[ii] );
      TEST( hpc::unmorton<3>( outputs[ii] )[0] == inputs_x[ii] );
      TEST( hpc::unmorton<3>( outputs[ii] )[1] == inputs_y[ii] );
      TEST( hpc::unmorton<3>( outputs[ii] )[2] == inputs_z[ii] );
   }
}
