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
#include "libhpc/debug/debug.hh"
#include "libhpc/containers/string.hh"
#include "libhpc/containers/vector.hh"
#include "libhpc/containers/po2_ring_buffer.hh"
#include "types.hh"

namespace hpc {
   namespace unix {

      class pipe
      {
      public:

         enum flags
         {
            none = 0,
            rdonly = O_RDONLY,
            wronly = O_WRONLY,
            rdwr = O_RDWR,
            nonblock = O_NONBLOCK,
            async = O_ASYNC,
            cloexec = O_CLOEXEC
         };

      public:

         pipe( int fd=-1,
               bool own=true );

         pipe( const string& pathname,
               pipe::flags flags=pipe::flags::rdonly );

         pipe( pipe&& src );

         ~pipe();

         void
         set_fd( int fd,
                 bool own=true );

         void
         open( const string& pathname,
               pipe::flags flags=pipe::flags::rdonly );

         void
         close();

         void
         add_flags( pipe::flags flags );

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
         void
         read( vector<T>& buf ) const
         {
            size_t size = read( (byte*)buf.data(), buf.capacity()*sizeof(T) );
            ASSERT( size%sizeof(T) == 0 );
            buf.resize( size/sizeof(T) );
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
      pipe::flags
      operator|( pipe::flags op0,
         pipe::flags op1 )
      {
         return pipe::flags( static_cast<int>( op0 ) | static_cast<int>( op1 ) );
      }
   }
}

#endif
