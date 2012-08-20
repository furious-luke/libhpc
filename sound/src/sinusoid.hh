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

#ifndef libhpc_sound_sinusoid_hh
#define libhpc_sound_sinusoid_hh

#include <math.h>

namespace hpc {
   namespace sound {
      namespace generator {

         class sinusoid
         {
         public:

            sinusoid( unsigned freq,
                      unsigned long rate );

            template< class Iterator >
            void
            operator()( Iterator start,
                        Iterator finish )
            {
               while( start != finish )
               {
                  *start = (typename Iterator::value_type)sin( 2.0*(float)_offs );
               }
            }

         protected:

            unsigned _freq;
            unsigned long _rate;
            unsigned long _offs;
         };
      }
   }
}

#endif
