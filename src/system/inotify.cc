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

#include <limits.h>
#include <libhpc/debug/insist.hh>
#include "inotify.hh"

namespace hpc {
   namespace os {

      inotify::inotify( file_descriptor::flags_type flags )
	 : file_descriptor(),
	   _size( 0 ),
	   _buf( nullptr )
      {
	 open( flags );
      }

      inotify::inotify( fs::path const& path,
			mask_type mask,
			file_descriptor::flags_type flags )
	 : file_descriptor(),
	   _size( 0 ),
	   _buf( nullptr )
      {
	 open( flags );
	 add_watch( path, mask );
      }

      void
      inotify::open( file_descriptor::flags_type flags )
      {
	 set_fd( inotify_init1( flags ) );
	 ASSERT( _fd >= 0, "Failed to initialise inotify." );
	 _size = sizeof(event_type) + NAME_MAX + 1;
	 _buf = new char[_size];
      }

      void
      inotify::close()
      {
	 file_descriptor::close();
	 _size = 0;
	 if( _buf )
	    delete[] _buf;
      }

      int
      inotify::add_watch( fs::path const& path,
			  mask_type mask )
      {
	 int wd = inotify_add_watch( _fd, path.native().c_str(), mask );
	 ASSERT( wd >= 0 );
	 return wd;
      }

      inotify::event_iterator
      inotify::event_begin()
      {
	 return event_iterator( this );
      }

      inotify::event_iterator
      inotify::event_end()
      {
	 return event_iterator();
      }

      inotify_event_iterator::inotify_event_iterator( inotify* in )
	 : _in( in ),
	   _n_bytes( -1 ),
	   _pos( 0 )
      {
	 if( _in )
	    _next();
      }

      void
      inotify_event_iterator::operator++()
      {
	 _pos += sizeof(inotify::event_type) + (**this).len;
	 if( _pos >= _n_bytes )
	    _next();
      }

      bool
      inotify_event_iterator::operator!=( inotify_event_iterator const& op )
      {
	 return _n_bytes != -1;
      }

      inotify::event_type&
      inotify_event_iterator::operator*()
      {
	 return *((inotify::event_type*)(_in->_buf + _pos));
      }

      void
      inotify_event_iterator::_next()
      {
	 _n_bytes = _in->read( _in->_buf, _in->_size );
	 ASSERT( _n_bytes >= 0 || errno == EAGAIN );
	 _pos = 0;
      }

   }
}

#endif
