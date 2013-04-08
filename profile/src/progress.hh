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

#ifndef libhpc_profile_progress_hh
#define libhpc_profile_progress_hh

#include "libhpc/hpcmpi/mpi.hh"

class progress_suite;

namespace hpc {
  namespace profile {

     class progress
     {
	friend class ::progress_suite;

     public:

	progress( const mpi::comm& comm = mpi::comm::world );

	void
	set_local_size( double size );

	void
	set_complete( double size );

	void
	set_delta( double delta );

	void
	update();

	double
	complete() const;

     protected:

	void
	_update_total( double delta );

     protected:

        shared_ptr<mpi::comm> _comm;
	int _root;
        double _update_fac;
	double _lsize, _gsize;
	double _comp, _old_comp;
	double _gcomp, _gsize_inv;
     };
  }
}

#endif
