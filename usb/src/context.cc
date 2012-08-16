#include "libhpc/debug/debug.hh"
#include "context.hh"

namespace hpc {
   namespace usb {

      context::context( bool local )
         : _ctx( NULL )
      {
         if( local )
            INSIST( libusb_init( &_ctx ), == 0 );
         else
            INSIST( libusb_init( NULL ), == 0 );
      }

      context::~context()
      {
         if( _ctx )
            libusb_exit( _ctx );
      }
   }
}
