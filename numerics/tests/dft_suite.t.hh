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

#include <math.h>
#include <cxxtest/TestSuite.h>
#include "libhpc/numerics/dft.hh"

#ifdef HAVE_FFTW3
using namespace hpc;
#endif

class dft_suite : public CxxTest::TestSuite {
public:

   void test_sine()
   {
#ifdef HAVE_FFTW3
      vector<numerics::dft::complex_type> in( 100 ), out( 100 );
      numerics::dft dft( in, out, numerics::dft::FORWARD );
      for( unsigned ii = 0; ii < in.size(); ++ii )
         in[ii] = std::complex<double>( sin( ((double)ii)/((double)(in.size() - 1))*4.0*2.0*M_PI ), 0.0 );
      dft.transform();
      std::cout << "\n" << in << "\n";
      std::cout << "\n" << out << "\n";
#endif
   }
};
