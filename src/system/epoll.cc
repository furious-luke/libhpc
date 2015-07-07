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

#ifndef DARWIN

#include <libhpc/debug/insist.hh>
#include <libhpc/debug/assert.hh>
#ifndef NDEBUG
#include <errno.h>
#endif
#include "epoll.hh"

namespace hpc {
   namespace os {

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

      uint32_t
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
	 : file_descriptor()
      {
	 open();
      }

      epoll::epoll( const file_descriptor& fd,
                    uint32_t events )
	 : file_descriptor()
      {
	 open();
         add( fd, events );
      }

      void
      epoll::open()
      {
         close();
         _fd = epoll_create1( 0 );
         ASSERT( _fd >= 0 );
      }

      void
      epoll::add( const file_descriptor& fd,
                  uint32_t events,
                  epoll_data_t data )
      {
         ASSERT( fd.fd() >= 0 );
         event_type event;

         // Set the events.
         event.events = events;

         // Set the data.
         event.data = data;

         // Add the event.
         INSIST( epoll_ctl( _fd, EPOLL_CTL_ADD, fd.fd(), &event ), >= 0 );
      }

      void
      epoll::add( int fd,
                  uint32_t events,
                  epoll_data_t data )
      {
         ASSERT( fd >= 0 );
         event_type event;

         // Set the events.
         event.events = events;

         // Set the data.
         event.data = data;

         // Add the event.
         INSIST( epoll_ctl( _fd, EPOLL_CTL_ADD, fd, &event ), >= 0 );
      }

      void
      epoll::add( const file_descriptor& fd,
                  uint32_t events )
      {
	 epoll_data_t data = {};
	 data.fd = fd.fd();
         add( fd, events, data );
      }

      void
      epoll::add( const file_descriptor& fd,
                  uint32_t events,
                  void* data )
      {
         epoll_data_t ed = {};
         ed.ptr = data;
         add( fd, events, ed );
      }

      void
      epoll::add( const file_descriptor& fd,
                  uint32_t events,
                  uint32_t data )
      {
         epoll_data_t ed = {};
         ed.u32 = data;
         add( fd, events, ed );
      }

      void
      epoll::add( int fd,
                  uint32_t events,
                  uint32_t data )
      {
	 epoll_data_t ed = {};
         ed.u32 = data;
         add( fd, events, ed );
      }

      void
      epoll::remove( const file_descriptor& fd )
      {
         ASSERT( fd.fd() >= 0 );
         INSIST( epoll_ctl( _fd, EPOLL_CTL_DEL, fd.fd(), NULL ), >= 0 );
      }

      void
      epoll::remove( int fd )
      {
         ASSERT( fd >= 0 );
         INSIST( epoll_ctl( _fd, EPOLL_CTL_DEL, fd, NULL ), >= 0 );
      }

      void
      epoll::modify( const file_descriptor& fd,
                     uint32_t events,
                     epoll_data_t data )
      {
         ASSERT( fd.fd() >= 0 );
         event_type event;

         // Set the events.
         event.events = events;

         // Set the data.
         event.data = data;

         // Add the event.
         INSIST( epoll_ctl( _fd, EPOLL_CTL_MOD, fd.fd(), &event ), >= 0 );
      }

      void
      epoll::modify( int fd,
                     uint32_t events,
                     epoll_data_t data )
      {
         ASSERT( fd >= 0 );
         event_type event;

         // Set the events.
         event.events = events;

         // Set the data.
         event.data = data;

         // Add the event.
         INSIST( epoll_ctl( _fd, EPOLL_CTL_MOD, fd, &event ), >= 0 );
      }

      void
      epoll::modify( const file_descriptor& fd,
                     uint32_t events )
      {
         epoll_data_t data = {};
         modify( fd, events, data );
      }

      void
      epoll::modify( const file_descriptor& fd,
                     uint32_t events,
                     void* data )
      {
         epoll_data_t ed = {};
         ed.ptr = data;
         modify( fd, events, ed );
      }

      void
      epoll::modify( const file_descriptor& fd,
                     uint32_t events,
                     uint32_t data )
      {
         epoll_data_t ed = {};
         ed.u32 = data;
         modify( fd, events, ed );
      }

      void
      epoll::modify( int fd,
                     uint32_t events,
                     uint32_t data )
      {
         epoll_data_t ed = {};
         ed.u32 = data;
         modify( fd, events, ed );
      }

      unsigned
      epoll::wait( std::vector<event_type>& events,
                   int timeout )
      {
         int ready = epoll_wait( _fd, events.data(), events.size(), timeout );
         ASSERT( ready >= 0, "epoll wait failed: ", strerror( errno ) );
         ASSERT( ready <= events.capacity() );
         return ready;
      }

   }
}

#endif
