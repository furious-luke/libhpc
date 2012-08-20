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
