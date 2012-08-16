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
