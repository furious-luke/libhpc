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

#include <stdlib.h>
#include <libhpc/containers/containers.hh>

int
main( int argc,
      char* argv[] )
{
   unsigned reps = (argc > 1 ) ? atoi( argv[1] ) : 1000;
   for( unsigned ii = 0; ii < reps; ++ii )
   {
      hpc::vector<int> vec( 1000 );
      vec.reserve( 10000 );
      vec.deallocate();
      vec.resize( 10000 );

      hpc::map<int,int> map;
      for( unsigned ii = 0; ii < 1000; ++ii )
	 map.insert( ii, ii );
      for( unsigned ii = 0; ii < 1000; ++ii )
      {
	 unsigned val;
	 val = map.get( ii );
	 val = map[ii];
      }

      hpc::multimap<int,int> mmap;
      for( unsigned ii = 0; ii < 1000; ++ii )
	 map.insert( ii/2, ii );
      for( unsigned ii = 0; ii < 1000; ++ii )
      {
	 auto rng = mmap.equal_range( ii/2 );
      }

      hpc::set<int> set;
      for( unsigned ii = 0; ii < 1000; ++ii )
	 set.insert( ii );
      for( unsigned ii = 0; ii < 1000; ++ii )
      {
	 bool has = set.has( ii );
      }
   }
   return EXIT_SUCCESS;
}
