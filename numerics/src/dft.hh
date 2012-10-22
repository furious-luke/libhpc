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

      ///
      ///
      ///
      template< class T >
      struct fftw_impl
      {
      };

      ///
      ///
      ///
      template<>
      struct fftw_impl<float>
      {
         typedef fftwf_plan plan_type;
         typedef float value_type;
         typedef fftwf_complex complex_type;
         typedef fftwf_iodim iodim_type;

         static inline fftwf_plan
         plan_dft_1d( int n, fftwf_complex* in, fftwf_complex* out, int sign, unsigned flags )
         {
            return fftwf_plan_dft_1d( n, in, out, sign, flags );
         }

         static inline fftwf_plan
         plan_dft_r2c_1d( int n, float* in, fftwf_complex* out, unsigned flags )
         {
            return fftwf_plan_dft_r2c_1d( n, in, out, flags );
         }

         static inline fftwf_plan
         plan_dft_c2r_1d( int n, fftwf_complex* in, float* out, unsigned flags )
         {
            return fftwf_plan_dft_c2r_1d( n, in, out, flags );
         }

         static inline plan_type
         plan_guru_dft( int rank, const iodim_type* dims, int howmany_rank, const iodim_type* howmany_dims,
                        complex_type* in, complex_type* out, int sign, unsigned flags )
         {
            return fftwf_plan_guru_dft( rank, dims, howmany_rank, howmany_dims, in, out, sign, flags );
         }

         static inline plan_type
         plan_guru_dft_r2c( int rank, const iodim_type* dims, int howmany_rank, const iodim_type* howmany_dims,
                            value_type* in, complex_type* out, unsigned flags )
         {
            return fftwf_plan_guru_dft_r2c( rank, dims, howmany_rank, howmany_dims, in, out, flags );
         }

         static inline plan_type
         plan_guru_dft_c2r( int rank, const iodim_type* dims, int howmany_rank, const iodim_type* howmany_dims,
                            complex_type* in, value_type* out, unsigned flags )
         {
            return fftwf_plan_guru_dft_c2r( rank, dims, howmany_rank, howmany_dims, in, out, flags );
         }

         static inline void
         destroy_plan( fftwf_plan p )
         {
            fftwf_destroy_plan( p );
         }

         static inline void
         execute( fftwf_plan p )
         {
            fftwf_execute( p );
         }
      };

      ///
      ///
      ///
      template<>
      struct fftw_impl<double>
      {
         typedef fftw_plan plan_type;
         typedef double value_type;
         typedef fftw_complex complex_type;
         typedef fftw_iodim iodim_type;

         static inline fftw_plan
         plan_dft_1d( int n, fftw_complex* in, fftw_complex* out, int sign, unsigned flags )
         {
            return fftw_plan_dft_1d( n, in, out, sign, flags );
         }

         static inline fftw_plan
         plan_dft_r2c_1d( int n, double* in, fftw_complex* out, unsigned flags )
         {
            return fftw_plan_dft_r2c_1d( n, in, out, flags );
         }

         static inline fftw_plan
         plan_dft_c2r_1d( int n, fftw_complex* in, double* out, unsigned flags )
         {
            return fftw_plan_dft_c2r_1d( n, in, out, flags );
         }

         static inline fftw_plan
         plan_guru_dft( int rank, const fftw_iodim* dims, int howmany_rank, const fftw_iodim* howmany_dims,
                        fftw_complex* in, fftw_complex* out, int sign, unsigned flags )
         {
            return fftw_plan_guru_dft( rank, dims, howmany_rank, howmany_dims, in, out, sign, flags );
         }

         static inline plan_type
         plan_guru_dft_r2c( int rank, const iodim_type* dims, int howmany_rank, const iodim_type* howmany_dims,
                            value_type* in, complex_type* out, unsigned flags )
         {
            return fftw_plan_guru_dft_r2c( rank, dims, howmany_rank, howmany_dims, in, out, flags );
         }

         static inline plan_type
         plan_guru_dft_c2r( int rank, const iodim_type* dims, int howmany_rank, const iodim_type* howmany_dims,
                            complex_type* in, value_type* out, unsigned flags )
         {
            return fftw_plan_guru_dft_c2r( rank, dims, howmany_rank, howmany_dims, in, out, flags );
         }

         static inline void
         destroy_plan( fftw_plan p )
         {
            fftw_destroy_plan( p );
         }

         static inline void
         execute( fftw_plan p )
         {
            fftw_execute( p );
         }
      };

      ///
      ///
      ///
      template< class T >
      class dft
      {
      public:

         typedef T value_type;
         typedef std::complex<value_type> complex_type;

         enum fft_type
         {
            FORWARD = FFTW_FORWARD,
            BACKWARD = FFTW_BACKWARD
         };

      public:

         dft()
            : _ready( false )
         {
         }

         dft( const typename vector<complex_type>::view& in,
              typename vector<complex_type>::view out,
              fft_type type=FORWARD )
            : _ready( false )
         {
            set_plan( in, out, type );
         }

         dft( const typename vector<value_type>::view& in,
              typename vector<complex_type>::view out,
              fft_type type=FORWARD,
              bool optimize=false,
              unsigned in_stride=1,
              unsigned out_stride=1 )
            : _ready( false )
         {
            set_plan( in, out, type );
         }

         ~dft()
         {
            clear();
         }

         void
         set_plan( const typename vector<complex_type>::view& in,
                   typename vector<complex_type>::view out,
                   fft_type type=FORWARD,
                   bool optimize=false )
         {
            ASSERT( in.size() == out.size() );
            unsigned flags = optimize ? FFTW_MEASURE : FFTW_ESTIMATE;
            _plan = fftw_impl<T>::plan_dft_1d( in.size(), (typename fftw_impl<T>::complex_type*)in.data(), (typename fftw_impl<T>::complex_type*)out.data(), type, flags );
            _ready = true;
         }

         void
         set_plan( const typename vector<value_type>::view& in,
                   typename vector<complex_type>::view out,
                   fft_type type=FORWARD,
                   bool optimize=false,
                   unsigned in_stride=1,
                   unsigned out_stride=1 )
         {
            ASSERT( in.size() == out.size() );
            unsigned flags = optimize ? FFTW_MEASURE : FFTW_ESTIMATE;
            if( type == FORWARD )
            {
               if( in_stride == 1 && out_stride == 1 )
                  _plan = fftw_impl<T>::plan_dft_r2c_1d( in.size(), in.data(), (typename fftw_impl<T>::complex_type*)out.data(), flags );
               else
               {
                  typename fftw_impl<T>::iodim_type dims, howmany_dims;
                  dims.n = 1;
                  dims.is = 1;
                  dims.os = 1;
                  howmany_dims.n = 1;
                  howmany_dims.is = in_stride;
                  howmany_dims.os = out_stride;
                  _plan = fftw_impl<T>::plan_guru_dft_r2c( 1, &dims, 1, &howmany_dims, in.data(), (typename fftw_impl<T>::complex_type*)out.data(), flags );
               }
            }
            else
            {
               if( in_stride == 1 && out_stride == 1 )
               {
                  _plan = fftw_impl<T>::plan_dft_c2r_1d( in.size(), (typename fftw_impl<T>::complex_type*)out.data(), in.data(), flags );
               }
               else
               {
                  typename fftw_impl<T>::iodim_type dims, howmany_dims;
                  dims.n = 1;
                  dims.is = in_stride;
                  dims.os = out_stride;
                  howmany_dims.n = 1;
                  howmany_dims.is = in_stride;
                  howmany_dims.os = out_stride;
                  _plan = fftw_impl<T>::plan_guru_dft_c2r( 1, &dims, 1, &howmany_dims, (typename fftw_impl<T>::complex_type*)out.data(), in.data(), flags );
               }
            }
            _ready = true;
         }

         void
         clear()
         {
            if( _ready )
            {
               fftw_impl<T>::destroy_plan( _plan );
               _ready = false;
            }
         }

         void
         transform()
         {
            fftw_impl<T>::execute( _plan );
         }

      protected:

         bool _ready;
         typename fftw_impl<T>::plan_type _plan;
      };
   }
}

#endif

#endif
