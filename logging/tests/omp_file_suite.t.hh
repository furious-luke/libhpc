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

#include <cxxtest/TestSuite.h>
#include "libhpc/logging/omp_file.hh"

using namespace hpc;

class omp_file_suite : public CxxTest::TestSuite {
public:

   void test_ctor()
   {
#if !defined(NLOG) && defined(_OPENMP)
      logging::omp::file log( "test.log." );
#endif
   }

   void test_one_thread()
   {
#if !defined(NLOG) && defined(_OPENMP)
      logging::omp::file log( "test.log." );
      log << "no indent" << setindent( 2 ) << logging::endl;
      log << "indent" << setindent( -2 ) << logging::endl;
      log << "no indent again" << logging::endl;
#endif
   }

   void test_three_threads()
   {
#if !defined(NLOG) && defined(_OPENMP)
      logging::omp::file log( "test.log." );
#pragma omp parallel num_threads(3)
      {
	 log << "no indent" << setindent( 2 ) << logging::endl;
	 log << "indent" << setindent( -2 ) << logging::endl;
	 log << "no indent again" << logging::endl;
      }
#endif
   }
};
