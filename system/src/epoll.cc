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
   namespace posix {

      int
      epoll::event_type::fd() const
      {
         return data.fd;
      }

      void*
      epoll::event_type::data_ptr() const
      {
         return data.ptr;
      }

      uint32
      epoll::event_type::data_uint32() const
      {
         return data.u32;
      }

      bool
      epoll::event_type::ready_in() const
      {
         return events & EPOLLIN;
      }

      bool
      epoll::event_type::ready_out() const
      {
         return events & EPOLLOUT;
      }

      bool
      epoll::event_type::error() const
      {
         return events & EPOLLERR;
      }

      bool
      epoll::event_type::hangup() const
      {
         return events & EPOLLHUP;
      }

      epoll::epoll()
         : _size( 0 )
      {
      }

      epoll::epoll( const pipe& pipe,
                    uint32_t events )
         : _size( 0 )
      {
         add( pipe, events );
      }

      void
      epoll::open()
      {
         close();
         _fd = epoll_create1( 0 );
         ASSERT( _fd >= 0 );
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
         OMP_SET_LOCK( _lock );
         ++_size;
         OMP_UNSET_LOCK( _lock );
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
         OMP_SET_LOCK( _lock );
         --_size;
         OMP_UNSET_LOCK( _lock );
      }

      void
      epoll::modify( const pipe& pipe,
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
         INSIST( epoll_ctl( _fd, EPOLL_CTL_MOD, pipe.fd(), &event ), >= 0 );
      }

      void
      epoll::modify( const pipe& pipe,
                     uint32_t events )
      {
         epoll_data_t data;
         modify( pipe, events, data );
      }

      void
      epoll::modify( const pipe& pipe,
                     uint32_t events,
                     void* data )
      {
         epoll_data_t ed;
         ed.ptr = data;
         modify( pipe, events, ed );
      }

      void
      epoll::modify( const pipe& pipe,
                     uint32_t events,
                     uint32 data )
      {
         epoll_data_t ed;
         ed.u32 = data;
         modify( pipe, events, ed );
      }

      void
      epoll::wait( vector<event_type>& events,
                   int timeout )
      {
         ASSERT( _size <= events.capacity() );
         events.resize( _size );
         unsigned ready = epoll_wait( _fd, events.data(), _size, timeout );
         events.resize( ready );
      }

   }
}
