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

      progress::progress()
	 : _root( 0 ),
	   _tag( 85804 ),
	   _max_probes( 20 )
      {
      }

      void
      progress::set_local_size( double size )
      {
	 ASSERT( _req.test(), "Can't resize progress without finishing prior sends." );
	 _lsize = size;
	 _comp = 0.0;
	 _old_comp = 0.0;

	 // Update global size.
	 _gsize = mpi::comm::world.all_reduce( _lsize );
	 _gsize_inv = 1.0/_gsize;
	 _gcomp = 0.0;
	 _rem = mpi::comm::world.size();
      }

      void
      progress::set_local_complete( double size )
      {
	 ASSERT( size <= _lsize, "Can't have finished more than maximum local size." );
	 _comp = size;
      }

      void
      progress::set_local_complete_delta( double delta )
      {
	 ASSERT( _comp + delta <= _lsize, "Can't have finished more than maximum local size." );
	 _comp += delta;
      }

      void
      progress::update()
      {
	 if( mpi::comm::world.rank() == _root )
	 {
	    // First update my local value.
	    if( _comp != _old_comp )
	    {
	       _update_total( _comp - _old_comp );
	       _old_comp = _comp;
	       if( num::approx( _comp, _lsize, 1e-3 ) )
	       {
		  ASSERT( _rem, "Internal error in progress tracker." );
		  --_rem;
	       }
	    }

	    // Now recieve from others.
	    MPI_Status stat;
	    for( unsigned ii = 0;
		 ii < _max_probes && mpi::comm::world.iprobe( stat, MPI_ANY_SOURCE, _tag );
		 ++ii )
	    {
	       double delta;
	       mpi::comm::world.recv( delta, stat.MPI_SOURCE, _tag );
	       if( delta == 0.0 )
	       {
		  ASSERT( _rem, "Internal error in progress tracker." );
		  --_rem;
	       }
	       else
		  _update_total( delta );
	    }
	 }
	 else
	 {
	    if( _req.test() )
	    {
	       if( _comp != _old_comp )
	       {
		  double delta = _comp - _old_comp;
		  mpi::comm::world.isend( delta, _root, _req, _tag );
		  _old_comp = _comp;
	       }
	    }
	 }
      }

      bool
      progress::test()
      {
	 if( mpi::comm::world.rank() == _root )
	    return _rem > 0;
	 else
	 {
	    // Finish most recent request.
	    _req.wait();

	    // Send updated completion.
	    if( _comp != _old_comp )
	    {
	       double delta = _comp - _old_comp;
	       mpi::comm::world.send( delta, _root, _tag );
	       _old_comp = _comp;
	    }

	    // Send terminate signal.
	    double delta = 0.0;
	    mpi::comm::world.send( delta, _root, _tag );

	    return false;
	 }
      }

      double
      progress::complete() const
      {
	 return _gcomp*_gsize_inv;
      }

      void
      progress::_update_total( double delta )
      {
	 _gcomp += delta;
	 if( _gcomp > _gsize )
	    _gcomp = _gsize;
      }
   }
}
