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

#include "libhpc/debug/assert.hh"
#include "timer.hh"

namespace hpc {
   namespace profile {

      timer::timer()
	 : _total( 0.0 ),
	   _cnt( 0 ),
	   _run( false ),
	   _stack( 0 )
      {
      }

      timer::~timer()
      {
	 ASSERT( !_stack );
	 ASSERT( !_run );
      }

      void
      timer::reset()
      {
	 _total = 0;
	 _cnt = 0;
      }

      bool
      timer::running() const
      {
	 return _run;
      }

      void
      timer::start()
      {
	 ++_stack;
	 if( !_run )
	 {
	    _run = true;
	    _start = unix::timer();
	 }
      }

      void
      timer::stop()
      {
	 --_stack;
	 if( !_stack )
	 {
	    _run = false;
	    _total += unix::seconds( unix::timer() - _start );
	 }
      }

      void
      timer::stop_tally()
      {
	 stop();
	 if( !_stack )
	    ++_cnt;
      }

      double
      timer::total() const
      {
	 return _total;
      }

      double
      timer::total( const mpi::comm& comm ) const
      {
	 return comm.all_reduce( _total, MPI_MAX );
      }

      double
      timer::mean() const
      {
	 ASSERT( _cnt );
	 return _total/(double)_cnt;
      }
   }
}
