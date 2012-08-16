#include "libhpc/debug/debug.hh"
#include "device_iterator.hh"

namespace hpc {
   namespace usb {

      device_iterator::device_iterator( context& ctx )
         : _ctx( ctx ),
           _list( NULL ),
           _size( 0 ),
           _idx( 0 )
      {
         begin();
      }

      device_iterator::~device_iterator()
      {
         if( _list )
            libusb_free_device_list( _list, 1 );
      }

      void
      device_iterator::begin()
      {
         _size = libusb_get_device_list( _ctx._ctx, &_list );
         _idx = 0;
      }

      bool
      device_iterator::done() const
      {
         return _idx >= _size;
      }

      void
      device_iterator::operator++()
      {
         ++_idx;
      }

      device
      device_iterator::operator*() const
      {
         ASSERT( _idx < _size && _list, "Invalid device list." );
         ASSERT( _list[_idx], "Invalid device." );
         return device( _list[_idx] );
      }
   }
}
