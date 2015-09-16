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

#ifndef hpc_system_file_descriptor_hh
#define hpc_system_file_descriptor_hh

#include <fcntl.h>
#include <errno.h>
#include <boost/filesystem.hpp>
#include "libhpc/containers/po2_ring_buffer.hh"
#include "filesystem.hh"

// Issue with some versions of CentOS.
#ifndef O_CLOEXEC
#define O_CLOEXEC 0
#endif

namespace hpc {
   namespace os {

      template< class S,
                class T >
      ssize_t
      read( S& strm,
            po2_ring_buffer<T>& buf )
      {
	 typename hpc::view<std::vector<T> > chunk = buf.first_vacant_chunk();
         ssize_t size = strm.read( (char*)chunk.data(), chunk.size()*sizeof(T) );
	 if( size > 0 )
	 {
	    ASSERT( size%sizeof(T) == 0 );
	    size /= sizeof(T);
	    if( size == chunk.size() )
	    {
	       chunk = buf.second_vacant_chunk();
	       if( chunk.size() )
	       {
		  ssize_t size2 = strm.read( (char*)chunk.data(), chunk.size()*sizeof(T) );
		  ASSERT( size2%sizeof(T) == 0 );
		  size2 /= sizeof(T);
		  size += size2;
	       }
	    }
	    buf.extend( size );
	 }
         return size;
      }

      template< class S >
      ssize_t
      write( S& strm,
             std::string const& buf )
      {
         return strm.write( buf.data(), buf.size() );
      }

      template< class S >
      ssize_t
      write( S& strm,
             void const* buf,
             size_t size )
      {
         return strm.write( buf, size );
      }

      class file_descriptor
      {
      public:

         enum flags_type
         {
            NONE = 0,
            READ_ONLY = O_RDONLY,
            WRITE_ONLY = O_WRONLY,
            READ_WRITE = O_RDWR,
            NONBLOCKING = O_NONBLOCK,
            ASYNCHRONOUS = O_ASYNC,
            CLOEXEC = O_CLOEXEC
         };

      public:

	 explicit
         file_descriptor( int fd = -1 );

         file_descriptor( fs::path const& pathname,
			  file_descriptor::flags_type flags = file_descriptor::READ_ONLY );

         file_descriptor( file_descriptor&& src ) noexcept;

         file_descriptor( file_descriptor const& src ) = delete;

         ~file_descriptor();

         void
         set_fd( int fd );

         void
         open( fs::path const& pathname,
               file_descriptor::flags_type flags = file_descriptor::READ_ONLY );

         void
         close();

         void
         release();

         void
         add_flags( file_descriptor::flags_type flags );

         int
         fd() const;

         ssize_t
         write( void const* buf,
                size_t size ) const;

         ssize_t
         write( std::string const& buf );

         ssize_t
         write( std::vector<char> const& buf );

         ssize_t
         read( void* buf,
               size_t size ) const;

         ssize_t
         read( std::vector<char>& buf ) const;

         template< class T >
         ssize_t
         read( po2_ring_buffer<T>& buf )
         {
            return os::read<file_descriptor,T>( *this, buf );
         }

         template< class T >
	 ssize_t
         write( std::vector<T>& buf ) const
         {
            return write( buf.data(), buf.size()*sizeof(T) );
         }

         bool
         operator==( const file_descriptor& op ) const;

         bool
         operator<( const file_descriptor& op ) const;

         file_descriptor&
         operator=( file_descriptor&& op ) noexcept;

         file_descriptor&
         operator=( file_descriptor const& op ) = delete;

      protected:

         int _fd;
      };

      inline
      file_descriptor::flags_type
      operator|( file_descriptor::flags_type op_a,
                 file_descriptor::flags_type op_b )
      {
         return file_descriptor::flags_type( static_cast<int>( op_a ) | static_cast<int>( op_b ) );
      }

   }
}

#endif
