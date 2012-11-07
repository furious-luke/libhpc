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
#include <libhpc/logging/logging.hh>

using namespace hpc;

int
main( int argc,
      char* argv[] )
{
   LOG_PUSH( new logging::stdout() );
   LOG_PUSH( new logging::file( "test." ) );
   unsigned reps = (argc > 1 ) ? atoi( argv[1] ) : 1000;
   for( unsigned ii = 0; ii < reps; ++ii )
   {
      LOGILN( "Test", ii );
   }
   LOG_POP();
   LOG_POP();

   LOG_PUSH( new logging::omp::file( "test." ) );
#pragma omp parallel num_threads(10)
   for( unsigned ii = 0; ii < reps; ++ii )
   {
      LOGILN( "Test", ii );
   }
   LOG_POP();

   return EXIT_SUCCESS;
}
