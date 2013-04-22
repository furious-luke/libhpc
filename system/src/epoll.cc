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

      const size_t epoll::default_max_events = 1024;

      epoll::epoll()
         : _size( 0 )
      {
      }

      epoll::epoll( const pipe& pipe,
                    uint32_t events,
                    size_t max_events )
         : _size( 0 )
      {
         open( max_events );
         add( pipe, events );
      }

      void
      epoll::open( size_t max_events )
      {
         close();
         _fd = epoll_create1( 0 );
         ASSERT( _fd >= 0 );
         _events.reallocate( max_events );
      }

      void
      epoll::add( const pipe& pipe,
                  uint32_t events,
                  epoll_data_t data )
      {
         ASSERT( pipe.fd() >= 0 );
         event_type event;

         // Set the events.
         event.events = events;

         // Set the data.
         event.data = data;

         // Add the event.
         INSIST( epoll_ctl( _fd, EPOLL_CTL_ADD, pipe.fd(), &event ), >= 0 );
         ++_size;
      }

      void
      epoll::add( const pipe& pipe,
                  uint32_t events )
      {
         epoll_data_t data;
         add( pipe, events, data );
      }

      void
      epoll::add( const pipe& pipe,
                  uint32_t events,
                  void* data )
      {
         epoll_data_t ed;
         ed.ptr = data;
         add( pipe, events, ed );
      }

      void
      epoll::add( const pipe& pipe,
                  uint32_t events,
                  uint32 data )
      {
         epoll_data_t ed;
         ed.u32 = data;
         add( pipe, events, ed );
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
         ASSERT( _size < _events.capacity() );
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
      epoll_event_iterator::ready_in() const
      {
         return (*this)->events & EPOLLIN;
      }

      bool
      epoll_event_iterator::ready_out() const
      {
         return (*this)->events & EPOLLOUT;
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
