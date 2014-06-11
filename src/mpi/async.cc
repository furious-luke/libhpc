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

#include "libhpc/system/has.hh"
#include "async.hh"

namespace hpc {
   namespace mpi {

      async_event_handler::async_event_handler( int tag )
         : _tag( tag )
      {
      }

      int
      async_event_handler::tag() const
      {
         return _tag;
      }

      async::async( mpi::comm const& comm )
         : _comm( &comm ),
           _max_evts( 0 )
      {
      }

      void
      async::set_comm( mpi::comm const& comm )
      {
         _comm = &comm;
         _ev_hndlrs.clear();
      }

      void
      async::set_max_events( unsigned max_evts )
      {
         _max_evts = max_evts;
      }

      void
      async::add_event_handler( async::event_handler* eh )
      {
         ASSERT( eh );
         ASSERT( !hpc::has( _ev_hndlrs, eh->tag() ) );
         _ev_hndlrs[eh->tag()] = eh;
      }

      std::unordered_map<int,async::event_handler*> const&
      async::event_handlers() const
      {
         return _ev_hndlrs;
      }

      bool
      async::run()
      {
         LOGBLOCKD( "Entering mpi::async run." );

         // If we are not the master, drop back out immediately.
         if( _comm->rank() != 0 )
         {
            LOGDLN( "Not async master, returning." );
            return false;
         }

	 // If we are running serially then skip this part to
         // avoid any kind of serial deadlock.
	 if( _comm->size() > 1 )
	 {
            unsigned n_evts = 0;
            MPI_Status stat;
            _n_done = 0;
            do
            {
               // Wait for anything.
               LOGDLN( "Probing for incoming events." );
               _comm->probe( stat );

	       LOGBLOCKD( "Have an incoming event from ", stat.MPI_SOURCE,
                          " with tag ", stat.MPI_TAG );

               // Be sure we have a handler for this.
               ASSERT( hpc::has( _ev_hndlrs, stat.MPI_TAG ) );

               // Run the event handler. A return value of positive indicates
               // that event handler is complete.
               if( _ev_hndlrs[stat.MPI_TAG]->event( stat ) )
                  ++_n_done;

               // If we've hit our event limit terminate.
               if( ++n_evts == _max_evts )
                  _n_done = _ev_hndlrs.size();
            }
            while( _n_done < _ev_hndlrs.size() );
	 }

         return true;
      }

   }
}
