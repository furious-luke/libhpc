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
#include <libhpc/debug.hh>
#include "file_descriptor.hh"

namespace hpc {
   namespace os {

      file_descriptor::file_descriptor( int fd )
         : _fd( fd )
      {
      }

      file_descriptor::file_descriptor( fs::path const& pathname,
					file_descriptor::flags_type flags )
         : _fd( -1 )
      {
         open( pathname, flags );
      }

      file_descriptor::file_descriptor( file_descriptor&& src ) noexcept
         : _fd( src._fd )
      {
         src._fd = -1;
      }

      file_descriptor::~file_descriptor()
      {
         close();
      }

      void
      file_descriptor::set_fd( int fd )
      {
         close();
         _fd = fd;
      }

      void
      file_descriptor::open( fs::path const& pathname,
			     file_descriptor::flags_type flags )
      {
         close();
         _fd = ::open( pathname.string().c_str(), static_cast<int>( flags ) );
         EXCEPT( _fd >= 0, "Unable to open file with path: ", pathname );
      }

      void
      file_descriptor::close()
      {
         if( _fd >= 0 )
         {
            INSIST( ::close( _fd ), == 0 );
            _fd = -1;
         }
      }

      void
      file_descriptor::release()
      {
         ASSERT( _fd >= 0, "Cannot release closed file descriptor." );
         _fd = -1;
      }

      void
      file_descriptor::add_flags( file_descriptor::flags_type flags )
      {
         int cur_flags = ::fcntl( _fd, F_GETFL, 0 );
         ASSERT( cur_flags >= 0 );
         cur_flags |= static_cast<int>( flags );
         INSIST( ::fcntl( _fd, F_SETFL, cur_flags ), == 0 );
      }

      int
      file_descriptor::fd() const
      {
         return _fd;
      }

      ssize_t
      file_descriptor::write( void const* buf,
			      size_t size ) const
      {
         ASSERT( _fd >= 0, "Invalid file descriptor." );
         return ::write( _fd, buf, size );
      }

      ssize_t
      file_descriptor::write( std::string const& buf )
      {
         return write( buf.data(), buf.size() );
      }

      ssize_t
      file_descriptor::write( std::vector<char> const& buf )
      {
         return write( buf.data(), buf.size() );
      }

      ssize_t
      file_descriptor::read( void* buf,
			     size_t size ) const
      {
         ASSERT( _fd >= 0, "Invalid file descriptor." );
         return ::read( _fd, buf, size );
      }

      ssize_t
      file_descriptor::read( std::vector<char>& buf ) const
      {
         return read( buf.data(), buf.size() );
      }

      bool
      file_descriptor::operator==( const file_descriptor& op ) const
      {
         return _fd == op._fd;
      }

      bool
      file_descriptor::operator<( const file_descriptor& op ) const
      {
         return _fd < op._fd;
      }

      file_descriptor&
      file_descriptor::operator=( file_descriptor&& op ) noexcept
      {
         _fd = op._fd;
         op._fd = -1;
         return *this;
      }

   }
}
