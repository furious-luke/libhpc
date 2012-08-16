#ifndef libhpc_usb_device_hh
#define libhpc_usb_device_hh

#include "libhpc/system/types.hh"
#include "context.hh"

namespace hpc {
   namespace usb {

      ///
      ///
      ///
      class device
      {
      public:

         device( libusb_device* device=NULL );

         ~device();

         hpc::uint16
         vendor() const;

         hpc::uint16
         product() const;

      protected:

         libusb_device* _dev;
         libusb_device_descriptor _desc;
      };
   }
}

#endif
