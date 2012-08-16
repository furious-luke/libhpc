#include "libhpc/debug/debug.hh"
#include "device.hh"

namespace hpc {
   namespace usb {

      device::device( libusb_device* device )
         : _dev( device )
      {
         if( _dev )
         {
            libusb_ref_device( _dev );
            libusb_get_device_descriptor( _dev, &_desc );
         }
      }

      device::~device()
      {
         if( _dev )
            libusb_unref_device( _dev );
      }

      uint16
      device::vendor() const
      {
         ASSERT( _dev );
         return _desc.idVendor;
      }

      uint16
      device::product() const
      {
         ASSERT( _dev );
         return _desc.idProduct;
      }
   }
}
