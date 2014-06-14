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

      mpi::comm const&
      async::comm() const
      {
	 return *_comm;
      }

      mpi::comm const&
      async::worker_comm() const
      {
	 return _wkr_comm;
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
	 ASSERT( eh->tag() != 0 );
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

	 // Compute the worker communicator.
	 if( _comm->size() > 1 )
	    _wkr_comm = _comm->create_excl( 0 );
	 else
	    _wkr_comm = mpi::comm::self;

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
	    int n_wkrs = _comm->size() - 1;
            unsigned n_evts = 0;
            MPI_Status stat;
            int n_done = 0;
            do
            {
               // Wait for anything.
               LOGDLN( "Probing for incoming events." );
               _comm->probe( stat );

	       LOGBLOCKD( "Have an incoming event from ", stat.MPI_SOURCE,
                          " with tag ", stat.MPI_TAG );

	       // A tag of zero indicates the worker is done.
	       if( stat.MPI_TAG == 0 )
	       {
		  int ec;
		  _comm->recv( ec, stat.MPI_SOURCE, stat.MPI_TAG );
		  ++n_done;
	       }
	       else
	       {
		  // Be sure we have a handler for this.
		  ASSERT( hpc::has( _ev_hndlrs, stat.MPI_TAG ) );

		  // Run the event handler. A return value of positive indicates
		  // that event handler has failed.
		  if( _ev_hndlrs[stat.MPI_TAG]->event( stat ) )
		     n_done = n_wkrs;

		  // If we've hit our event limit terminate.
		  if( ++n_evts == _max_evts )
		     n_done = n_wkrs;
	       }
            }
            while( n_done < n_wkrs );
	 }
	 else
	   return false;

         return true;
      }

      void
      async::done( int ec ) const
      {
	 if( _comm->size() > 1 )
	 {
	    ASSERT( _comm->rank() > 0 );
	    _comm->send( ec, 0, 0 );
	 }
      }

   }
}
