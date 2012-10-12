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

#include "dft.hh"

#ifdef HAVE_FFTW3

namespace hpc {
   namespace numerics {

      dft::dft()
         : _ready( false )
      {
      }

      dft::dft( const vector<complex_type>::view& in,
                vector<complex_type>::view out,
                fft_type type )
         : _ready( false )
      {
         set_plan( in, out, type );
      }

      dft::~dft()
      {
         clear();
      }

      void
      dft::set_plan( const vector<complex_type>::view& in,
                     vector<complex_type>::view out,
                     fft_type type )
      {
         ASSERT( in.size() == out.size() );
         _plan = fftw_plan_dft_1d( in.size(), (fftw_complex*)in.data(), (fftw_complex*)out.data(), type, FFTW_ESTIMATE );
         _ready = true;
      }

      void
      dft::clear()
      {
         if( _ready )
         {
            fftw_destroy_plan( _plan );
            _ready = false;
         }
      }

      void
      dft::transform()
      {
         fftw_execute( _plan );
      }
   }
}

#endif
