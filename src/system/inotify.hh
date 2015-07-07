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

#ifndef hpc_system_inotify_hh
#define hpc_system_inotify_hh

#ifndef DARWIN

#include <sys/inotify.h>
#include "file_descriptor.hh"

namespace hpc {
   namespace os {

      class inotify_event_iterator;

      class inotify
         : public file_descriptor
      {
         friend class inotify_event_iterator;

      public:

	 typedef struct inotify_event event_type;
	 typedef inotify_event_iterator event_iterator;

	 enum mask_type
	 {
	    CREATE = IN_CREATE,
	    MODIFY = IN_MODIFY
	 };

      public:

         inotify( file_descriptor::flags_type flags = file_descriptor::NONBLOCKING );

         inotify( fs::path const& path,
		  mask_type mask,
		  file_descriptor::flags_type flags = file_descriptor::NONBLOCKING );

         void
         open( file_descriptor::flags_type flags = file_descriptor::NONBLOCKING );

	 void
	 close();

	 int
         add_watch( fs::path const& path,
		    mask_type mask );

	 event_iterator
	 event_begin();

	 event_iterator
	 event_end();

      protected:

	 unsigned _size;
	 char* _buf;
      };

      class inotify_event_iterator
      {
      public:

	 inotify_event_iterator( inotify* in = nullptr );

	 void
	 operator++();

	 bool
	 operator!=( inotify_event_iterator const& op );

	 inotify::event_type&
	 operator*();

      protected:

	 void
	 _next();

      protected:

	 inotify* _in;
	 ssize_t _n_bytes, _pos;
      };

   }
}

#endif

#endif
