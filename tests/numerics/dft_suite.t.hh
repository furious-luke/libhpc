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

   void test_sine_basic()
   {
#ifdef HAVE_FFTW3
      vector<numerics::dft<float>::complex_type> in( 100 ), out( 100 );
      numerics::dft<float> dft( in, out, numerics::dft<float>::FORWARD );
      numerics::dft<float> inv( out, in, numerics::dft<float>::BACKWARD );
      for( unsigned ii = 0; ii < in.size(); ++ii )
         in[ii] = std::complex<double>( sin( ((double)ii)/((double)in.size())*2.0*M_PI ), 0.0 );
      dft.transform();

      // for( unsigned ii = 0; ii < out.size(); ++ii )
      //    std::cout << 1.0/(100.0 - (double)ii) << ", " << std::abs( out[ii] ) << "\n";

      inv.transform();
      for( unsigned ii = 0; ii < in.size(); ++ii )
         in[ii] = std::complex<double>( in[ii].real()/(double)in.size(), in[ii].imag() );
#endif
   }

   void test_sine_real_interleaved()
   {
#ifdef HAVE_FFTW3
      vector<numerics::dft<float>::value_type> in( 200 );
      vector<numerics::dft<float>::complex_type> out( 200 );
      numerics::dft<float> dft( in, out, numerics::dft<float>::FORWARD, false, 2, 2 );
      numerics::dft<float> inv( in, out, numerics::dft<float>::BACKWARD, false, 2, 2 );

      for( unsigned ii = 0; ii < in.size(); ii += 2 )
         in[ii] = sin( ((double)ii)/((double)in.size())*2.0*M_PI );
      for( unsigned ii = 1; ii < in.size(); ii += 2 )
         in[ii] = sin( ((double)ii)/((double)in.size())*2.0*M_PI );

      dft.transform();

      for( unsigned ii = 0; ii < out.size(); ii += 2 )
         std::cout << 1.0/(100.0 - (double)(ii/2)) << ", " << std::abs( out[ii] ) << "\n";

      inv.transform();

      for( unsigned ii = 0; ii < in.size(); ++ii )
         in[ii] = in[ii]/(double)(in.size()/2);
#endif
   }
};
