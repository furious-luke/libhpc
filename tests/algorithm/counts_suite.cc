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

#include <numeric>
#include <libhpc/unit_test/main.hh>
#include <libhpc/algorithm/counts.hh>

TEST_CASE( "/hpc/algorithm/counts_to_displs" )
{
   std::vector<int> cnts( 10 ), displs;
   std::iota( cnts.begin(), cnts.end(), 0 );

   displs.resize( cnts.size() + 1 );
   hpc::counts_to_displs( cnts.begin(), cnts.end(), displs.begin() );
   int val = 0;
   for( int ii = 0; ii < 11; ++ii )
   {
      TEST( displs[ii] == val );
      if( ii < cnts.size() )
         val += cnts[ii];
   }
}

TEST_CASE( "/hpc/algorithm/counts_to_displs/inplace" )
{
   std::vector<int> vec( 11 );
   for( int ii = 0; ii < 10; ++ii )
      vec[ii] = ii;

   hpc::counts_to_displs( vec.begin(), 10 );
   int val = 0;
   for(int ii = 0; ii < 11; ++ii)
   {
      TEST( vec[ii] == val );
      if( ii < 10 )
         val += ii;
   }
}

TEST_CASE( "/hpc/algorithm/displs_to_counts" )
{
   std::vector<int> cnts( 10 ), displs( 11 );
   int val = 0;
   for( int ii = 0; ii < 11; ++ii )
   {
      displs[ii] = val;
      val += ii;
   }

   hpc::displs_to_counts( displs.begin(), displs.end(), cnts.begin() );
   for( int ii = 0; ii < 10; ++ii )
      TEST( cnts[ii] == ii );
}

TEST_CASE( "/hpc/algorithm/displs_to_counts/inplace" )
{
   std::vector<int> vec( 11 );
   vec[0] = 0;
   for( int ii = 0; ii < 10; ++ii )
      vec[ii + 1] = vec[ii] + ii;

   hpc::displs_to_counts( vec.begin(), 11 );
   for( int ii = 0; ii < 10; ++ii )
      TEST( vec[ii] == ii );
}

TEST_CASE( "/hpc/algorithm/correct_displs" )
{
   std::vector<int> vec( 4 );
   vec[0] = 2; vec[1] = 5; vec[2] = 12; vec[3] = 22;
   hpc::correct_displs<std::vector<int> >( vec );
   TEST( vec[0] == 0 );
   TEST( vec[1] == 2 );
   TEST( vec[2] == 5 );
   TEST( vec[3] == 12 );
}
