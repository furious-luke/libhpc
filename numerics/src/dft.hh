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

#ifndef libhpc_numerics_dft_hh
#define libhpc_numerics_dft_hh

#ifdef HAVE_FFTW3

#include <complex>
#include <fftw3.h>
#include "libhpc/debug/debug.hh"
#include "libhpc/containers/vector.hh"

namespace hpc {
   namespace numerics {

      class dft
      {
      public:

         // typedef fftw_complex complex_type;
         typedef std::complex<double> complex_type;

         enum fft_type
         {
            FORWARD = FFTW_FORWARD,
            BACKWARD = FFTW_BACKWARD
         };

      public:

         dft();

         dft( const vector<complex_type>::view& in,
              vector<complex_type>::view out,
              fft_type type=FORWARD );

         ~dft();

         void
         set_plan( const vector<complex_type>::view& in,
                   vector<complex_type>::view out,
                   fft_type type=FORWARD );

         void
         clear();

         void
         transform();

      protected:

         bool _ready;
         fftw_plan _plan;
      };
   }
}

#endif

#endif
