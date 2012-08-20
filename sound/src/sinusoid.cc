#include "sinusoid.hh"

namespace hpc {
   namespace sound {
      namespace generator {

         sinusoid::sinusoid( unsigned freq,
            unsigned long rate )
            : _freq( freq ),
              _rate( rate )
         {
         }
      }
   }
}
