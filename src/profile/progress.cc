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

#include "progress.hh"

namespace hpc {
   namespace profile {

      progress::progress( const mpi::comm& comm )
	 : _comm( comm ),
           _root( 0 ),
           _update_fac( 0.1 )
      {
      }

      void
      progress::set_local_size( double size )
      {
	 _lsize = size;
	 _comp = 0.0;
	 _old_comp = 0.0;

	 // Update global size.
	 _gsize = _comm->all_reduce( _lsize );
	 _gsize_inv = 1.0/_gsize;
	 _gcomp = 0.0;
      }

      void
      progress::set_complete( double size )
      {
	 ASSERT( size <= _lsize, "Can't have finished more than maximum local size." );
	 _comp = size;
      }

      void
      progress::set_delta( double delta )
      {
	 ASSERT( _comp + delta <= _lsize, "Can't have finished more than maximum local size." );
	 _comp += delta;
      }

      void
      progress::update()
      {
         unsigned num_updates = (_comp - _old_comp)/_update_fac;
         LOGTLN( "Have ", num_updates, " updates to do." );
         for( unsigned ii = 0; ii < num_updates; ++ii )
         {
            LOGTLN( "Updating." );
            _gcomp = _comm->all_reduce( _comp );
         }
         _old_comp = _comp;
      }

      // void
      // progress::update()
      // {
      //    if( _rank == 0 )
      //    {
      //       // Process all incoming updates.
      //       MPI_Status stat;
      //       while( _comm.iprobe( stat, MPI_ANY_SOURCE, _tag ) )
      //       {
      //          _comm.recv(
      //       }
      //    }
      //    else
      //    {
      //       // I would like to cancel any ongoing send, but that is
      //       // usually very expensive. Instead just ignore this send.
      //       if( !_req )
      //       {
      //          // Only perform a send if my completion is different.
      //          if( _comp != _old_comp )
      //          {
      //             _comm.isend( _comp, _root, _req, _tag );
      //             _old_comp = _comp;
      //          }
      //       }
      //    }
      // }

      double
      progress::complete() const
      {
	 return _gcomp*_gsize_inv;
      }
   }
}
