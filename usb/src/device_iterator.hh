#ifndef libhpc_usb_device_iterator_hh
#define libhpc_usb_device_iterator_hh

#include "device.hh"

namespace hpc {
   namespace usb {

      ///
      ///
      ///
      class device_iterator
      {
      public:

         device_iterator( context& ctx );

         ~device_iterator();

         void
         begin();

         bool
         done() const;

         void
         operator++();

         device
         operator*() const;

      protected:

         context& _ctx;
         libusb_device** _list;
         unsigned _size;
         unsigned _idx;
      };
   }
}

#endif
