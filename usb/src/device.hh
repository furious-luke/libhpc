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

         void
         set_device( libusb_device* dev );

         bool
         claim();

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
