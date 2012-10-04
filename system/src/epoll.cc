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

#include "epoll.hh"

namespace hpc {
   namespace unix {

      epoll::epoll()
         : _size( 0 ),
           _max_events( 64 )
      {
      }

      epoll::epoll( const pipe& pipe )
         : _size( 0 ),
           _max_events( 64 )
      {
         open();
         add( pipe );
      }

      void
      epoll::open()
      {
         close();
         _fd = epoll_create1( 0 );
         ASSERT( _fd >= 0 );
         _events.reallocate( _max_events );
      }

      void
      epoll::add( const pipe& pipe,
                  bool edge,
                  void* data )
      {
         ASSERT( pipe.fd() >= 0 );
         event_type event;

         // Set the events.
         event.events = EPOLLIN | EPOLLPRI | EPOLLERR | EPOLLHUP;
         if( edge )
            event.events |= EPOLLET;

         // The data is a union, set accordingly.
         if( data )
            event.data.ptr = data;
         else
            event.data.fd = pipe.fd();

         // Add the event.
         INSIST( epoll_ctl( _fd, EPOLL_CTL_ADD, pipe.fd(), &event ), >= 0 );
         ++_size;
      }

      void
      epoll::remove( const pipe& pipe )
      {
         ASSERT( pipe.fd() >= 0 );
         INSIST( epoll_ctl( _fd, EPOLL_CTL_DEL, pipe.fd(), NULL ), >= 0 );
         --_size;
      }

      void
      epoll::wait( int timeout )
      {
         _events.resize( _size );
         unsigned ready = epoll_wait( _fd, _events.data(), _size, timeout );
         _events.resize( ready );
      }

      epoll::iterator
      epoll::begin() const
      {
         return _events.begin();
      }

      epoll::iterator
      epoll::end() const
      {
         return _events.end();
      }

      epoll_event_iterator::epoll_event_iterator( vector<epoll::event_type>::const_iterator it )
         : epoll_event_iterator::iterator_adaptor_( it )
      {
      }

      int
      epoll_event_iterator::fd() const
      {
         return (*this)->data.fd;
      }

      void*
      epoll_event_iterator::data() const
      {
         return (*this)->data.ptr;
      }

      bool
      epoll_event_iterator::ready() const
      {
         return (*this)->events & EPOLLIN;
      }

      bool
      epoll_event_iterator::error() const
      {
         return (*this)->events & EPOLLERR;
      }

      bool
      epoll_event_iterator::hangup() const
      {
         return (*this)->events & EPOLLHUP;
      }
   }
}
