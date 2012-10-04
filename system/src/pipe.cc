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

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "pipe.hh"

namespace hpc {
   namespace unix {

      pipe::pipe( int fd,
                  bool own )
         : _fd( fd ),
           _own( own )
      {
      }

      pipe::pipe( const string& pathname,
                  pipe::flags flags )
         : _fd( -1 ),
           _own( true )
      {
         open( pathname, flags );
      }

      pipe::~pipe()
      {
      }

      void
      pipe::set_fd( int fd,
                    bool own )
      {
         close();
         _fd = fd;
         _own = own;
      }

      void
      pipe::open( const string& pathname,
                  pipe::flags flags )
      {
         close();
         _fd = ::open( pathname.c_str(), static_cast<int>( flags ) );
         ASSERT( _fd >= 0 );
         _own = true;
      }

      void
      pipe::close()
      {
         if( _fd >= 0 && _own )
            INSIST( ::close( _fd ), == 0 );
         _fd = -1;
      }

      void
      pipe::add_flags( pipe::flags flags )
      {
         int cur_flags = ::fcntl( _fd, F_GETFL, 0 );
         ASSERT( cur_flags >= 0 );
         cur_flags |= static_cast<int>( flags );
         INSIST( ::fcntl( _fd, F_SETFL, cur_flags ), == 0 );
      }

      int
      pipe::fd() const
      {
         return _fd;
      }

      void
      pipe::write( const byte* buf,
                   size_t size ) const
      {
         ASSERT( _fd >= 0 );
         ASSERT( !size || buf );
         ssize_t res = ::write( _fd, buf, size );
         ASSERT( res >= 0 );
         ASSERT( res == size );
      }

      ssize_t
      pipe::read( byte* buf,
                  size_t size ) const
      {
         ASSERT( _fd >= 0 );
         ssize_t res = ::read( _fd, buf, size );
         if( res == -1 && errno == EAGAIN )
            res = 0;
         ASSERT( res >= 0 );
         ASSERT( res <= size );
         return res;
      }

      void
      pipe::read( string& buf ) const
      {
         buf.resize( buf.capacity() );
         unsigned size = read( (byte*)buf.data(), buf.size()*sizeof(char) );
         ASSERT( size%sizeof(char) == 0 );
         buf.resize( size/sizeof(char) );
      }
   }
}
