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

#ifdef DARWIN

#include <sys/time.h>
#include <libhpc/debug/assert.hh>
#include "kqueue.hh"

namespace hpc {
   namespace os {

      int
      kqueue::event_type::fd() const
      {
         return ident;
      }

      uint32
      kqueue::event_type::data_uint32() const
      {
         return udata;
      }

      bool
      kqueue::event_type::ready_in() const
      {
         return filter == EVFILT_READ;
      }

      bool
      kqueue::event_type::ready_out() const
      {
         return filter == EVFILT_WRITE;
      }

      bool
      kqueue::event_type::hangup() const
      {
         return flags & EV_EOF;
      }

      kqueue::kqueue()
         : file_descriptor()
      {
      }

      void
      kqueue::open()
      {
         close();
         _fd = ::kqueue();
         EXCEPT( _fd >= 0, "Failed to initialise kqueue descriptor." );
      }

      void
      kqueue::add( file_descriptor const& fd,
                   int16_t filter,
		   uint32_t fflags,
                   uint32_t data,
                   bool oneshot,
                   bool clear )
      {
	 add( fd.fd(), filter, fflags, data, oneshot, clear );
      }

      void
      kqueue:add( process const& proc,
		  uint32_t fflags,
		  uint32_t data,
		  bool oneshot,
		  bool clear )
      {
	 add( proc.pid(), EVFILT_PROC, fflags, data, oneshot, clear );
      }

      void
      kqueue::add( int fd,
                   int16_t filter,
		   uint32_t fflags,
                   uint32_t data,
                   bool oneshot,
                   bool clear )
      {
         ASSERT( fd >= 0 );
         struct kevent64_s event;
         uint16_t flags = EV_ADD;
         if( oneshot )
            flags |= EV_ONESHOT;
         if( clear )
            flags |= EV_CLEAR;
         EV_SET64( &event, fd, filter, flags, fflags, 0, (uint64_t)data, 0, 0 );
         INSIST( kevent64( _fd, &event, 1, 0, 0, 0, 0 ), == 0 );
      }

      void
      kqueue::remove( file_descriptor const& fd,
                      int16_t filter )
      {
         ASSERT( fd.fd() >= 0 );
         struct kevent64_s event;
         EV_SET64( &event, fd.fd(), filter, EV_DELETE, 0, 0, 0, 0, 0 );
         INSIST( kevent64( _fd, &event, 1, 0, 0, 0, 0 ), == 0 );
      }

      void
      kqueue::remove( int fd,
                      int16_t filter )
      {
         ASSERT( fd >= 0 );
         struct kevent64_s event;
         EV_SET64( &event, fd, filter, EV_DELETE, 0, 0, 0, 0, 0 );
         INSIST( kevent64( _fd, &event, 1, 0, 0, 0, 0 ), == 0 );
      }

      unsigned
      kqueue::wait( std::vector<event_type>& events,
                    int timeout )
      {
         int ready;
         if( timeout >= 0 )
         {
            struct timespec ts = (struct timespec){ 0 };
            ts.tv_sec = timeout/1000;
            ts.tv_nsec = (long)(timeout%1000)*1000L;
            ready = kevent64( _fd, 0, 0, events.data(), events.size(), 0, &ts );
         }
         else
            ready = kevent64( _fd, 0, 0, events.data(), events.size(), 0, 0 );

         ASSERT( ready >= 0 );
         ASSERT( ready <= events.capacity() );
         return ready;
      }

   }
}

#endif
