#ifndef libhpc_usb_context_hh
#define libhpc_usb_context_hh

#include <libusb-1.0/libusb.h>

namespace hpc {
   namespace usb {

      class device_iterator;

      ///
      ///
      ///
      class context
      {
      public:

         context( bool local=false );

         ~context();

      protected:

         libusb_context* _ctx;

         friend class device_iterator;
      };
   }
}

#endif
