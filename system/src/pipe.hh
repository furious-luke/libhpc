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

#ifndef libhpc_system_pipe_hh
#define libhpc_system_pipe_hh

#include <fcntl.h>
#include <errno.h>
#include "libhpc/debug/debug.hh"
#include "libhpc/containers/string.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/po2_ring_buffer.hh"
#include "types.hh"
#include "gcc_4.6_fix.hh"
#include "cc_version.hh"

namespace hpc {
   namespace posix {

      class pipe
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

         pipe( int fd = -1,
               bool own = true );

         pipe( const string& pathname,
               pipe::flags_type flags = pipe::READ_ONLY );

#if CXX_0X

         pipe( pipe&& src );

#endif

         ~pipe();

         void
         set_fd( int fd,
                 bool own = true );

         void
         open( const string& pathname,
               pipe::flags_type flags = pipe::READ_ONLY );

         void
         close();

         void
         add_flags( pipe::flags_type flags );

         int
         fd() const;

         void
         write( const byte* buf,
                size_t size ) const;

         void
         write( const string& buf );

         size_t
         read( byte* buf,
               size_t size ) const;

         void
         read( string& buf ) const;

         template< class T >
         void
         write( vector<T>& buf ) const
         {
            write( buf.data(), buf.size()*sizeof(T) );
         }

         template< class T >
         size_t
         read( vector<T>& buf ) const
         {
            buf.resize( buf.capacity() );
            size_t size = read( (byte*)buf.data(), buf.capacity()*sizeof(T) );
            ASSERT( size%sizeof(T) == 0 );
            buf.resize( size/sizeof(T) );
            return size;
         }

         template< class T >
         size_t
         read( po2_ring_buffer<T>& buf ) const
         {
            typename vector<T>::view chunk = buf.first_vacant_chunk();
            ssize_t size = read( (byte*)chunk.data(), chunk.size()*sizeof(T) );
            if( ISERR( EAGAIN ) )
               return 0;
            ASSERT( size%sizeof(T) == 0 );
            size /= sizeof(T);
            if( size == chunk.size() )
            {
               chunk = buf.second_vacant_chunk();
               if( chunk.size() )
               {
                  ssize_t size2 = read( (byte*)chunk.data(), chunk.size()*sizeof(T) );
                  ASSERT( size2%sizeof(T) == 0 );
                  size2 /= sizeof(T);
                  size += size2;
               }
            }
            buf.extend( size );
            return size;
         }

         bool
         operator==( const pipe& op ) const;

         bool
         operator<( const pipe& op ) const;

      protected:

         int _fd;
         bool _own;
      };

      inline
      pipe::flags_type
      operator|( pipe::flags_type op_a,
                 pipe::flags_type op_b )
      {
         return pipe::flags_type( static_cast<int>( op_a ) | static_cast<int>( op_b ) );
      }

   }
}

#endif
