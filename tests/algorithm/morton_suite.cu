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

TEST_CASE_CUDA( "/hpc/algorithm/morton/dilate/2d" )
{
   uint16_t inputs[] = { BOOST_BINARY( 00000000 00000000 ), BOOST_BINARY( 00000000 00000001 ),
                         BOOST_BINARY( 01010101 01010101 ), BOOST_BINARY( 10101010 10101010 ),
                         BOOST_BINARY( 11111111 11111111 ) };
   uint32_t outputs[] = { BOOST_BINARY( 00000000 00000000 00000000 00000000 ),
                          BOOST_BINARY( 00000000 00000000 00000000 00000001 ),
                          BOOST_BINARY( 00010001 00010001 00010001 00010001 ),
                          BOOST_BINARY( 01000100 01000100 01000100 01000100 ),
                          BOOST_BINARY( 01010101 01010101 01010101 01010101 ) };
   unsigned size = sizeof(inputs)/sizeof(inputs[0]);
   for( unsigned ii = 0; ii < size; ++ii )
   {
      TEST_EQ( hpc::dilate<2>( inputs[ii] ), outputs[ii] );
      TEST_EQ( hpc::undilate<2>( outputs[ii] ), inputs[ii] );
   }
}

TEST_CASE_CUDA( "/hpc/algorithm/morton/dilate/3d" )
{
   uint16_t inputs[] = { BOOST_BINARY( 00000000 00000000 ), BOOST_BINARY( 00000000 00000001 ),
                         BOOST_BINARY( 00000001 01010101 ), BOOST_BINARY( 00000000 10101010 ),
                         BOOST_BINARY( 00000011 11111111 ) };
   uint32_t outputs[] = { BOOST_BINARY( 00000000 00000000 00000000 00000000 ),
                          BOOST_BINARY( 00000000 00000000 00000000 00000001 ),
                          BOOST_BINARY( 00000001 00000100 00010000 01000001 ),
                          BOOST_BINARY( 00000000 00100000 10000010 00001000 ),
                          BOOST_BINARY( 00001001 00100100 10010010 01001001 ) };
   unsigned size = sizeof(inputs)/sizeof(inputs[0]);
   for( unsigned ii = 0; ii < size; ++ii )
   {
      TEST_EQ( hpc::dilate<3>( inputs[ii] ), outputs[ii] );
      TEST_EQ( hpc::undilate<3>( outputs[ii] ), inputs[ii] );
   }
}

TEST_CASE( "/hpc/algorithm/morton/2d" )
{
   uint16_t inputs_x[] = { BOOST_BINARY( 00000000 00000000 ), BOOST_BINARY( 00000000 00000001 ),
                           BOOST_BINARY( 01010101 01010101 ), BOOST_BINARY( 10101010 10101010 ),
                           BOOST_BINARY( 11111111 11111111 ), BOOST_BINARY( 00101000 11010011 ) };
   uint16_t inputs_y[] = { BOOST_BINARY( 00000000 00000000 ), BOOST_BINARY( 00000000 00000001 ),
                           BOOST_BINARY( 01010101 01010101 ), BOOST_BINARY( 10101010 10101010 ),
                           BOOST_BINARY( 11111111 11111111 ), BOOST_BINARY( 01110101 00110100 ) };
   uint32_t outputs[] = { BOOST_BINARY( 00000000 00000000 00000000 00000000 ),
                          BOOST_BINARY( 00000000 00000000 00000000 00000011 ),
                          BOOST_BINARY( 00110011 00110011 00110011 00110011 ),
                          BOOST_BINARY( 11001100 11001100 11001100 11001100 ),
                          BOOST_BINARY( 11111111 11111111 11111111 11111111 ),
                          BOOST_BINARY( 00101110 01100010 01011011 00100101 ) };
   unsigned size = sizeof(outputs)/sizeof(outputs[0]);
   for( unsigned ii = 0; ii < size; ++ii )
   {
      TEST_EQ( hpc::morton<2>( inputs_x[ii], inputs_y[ii] ), outputs[ii] );
      TEST_EQ( hpc::unmorton<2>( outputs[ii] )[0], inputs_x[ii] );
      TEST_EQ( hpc::unmorton<2>( outputs[ii] )[1], inputs_y[ii] );
   }
}

TEST_CASE( "/hpc/algorithm/morton/3d" )
{
   uint16_t inputs_x[] = { BOOST_BINARY( 00000000 00001001 ) };
   uint16_t inputs_y[] = { BOOST_BINARY( 00000000 00000101 ) };
   uint16_t inputs_z[] = { BOOST_BINARY( 00000000 00001001 ) };
   uint32_t outputs[] = { BOOST_BINARY( 00000000 00000000 00001010 10000111 ) };
   unsigned size = sizeof(outputs)/sizeof(outputs[0]);
   for( unsigned ii = 0; ii < size; ++ii )
   {
      TEST_EQ( hpc::morton<3>( inputs_x[ii], inputs_y[ii], inputs_z[ii] ), outputs[ii] );
      TEST_EQ( hpc::unmorton<3>( outputs[ii] )[0], inputs_x[ii] );
      TEST_EQ( hpc::unmorton<3>( outputs[ii] )[1], inputs_y[ii] );
      TEST_EQ( hpc::unmorton<3>( outputs[ii] )[2], inputs_z[ii] );
   }
}
