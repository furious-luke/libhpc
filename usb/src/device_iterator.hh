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

         template< class Device >
         void
         device( Device& dev ) const
         {
            ASSERT( _idx < _size && _list, "Invalid device list." );
            ASSERT( _list[_idx], "Invalid device." );
            dev.set_device( _list[_idx] );
         }

      protected:

         context& _ctx;
         libusb_device** _list;
         unsigned _size;
         unsigned _idx;
      };
   }
}

#endif
