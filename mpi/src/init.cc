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

#include "init.hh"
#include "data_type.hh"
#include "comm.hh"
#include "insist.hh"
#include "logger.hh"

namespace hpc {
   namespace mpi {

      bool initialized = false;

      void
      initialise()
      {
	 int argc = 0;
	 char** argv = NULL;
	 initialise( argc, argv );
      }

      void
      initialise( int& argc,
                  char**& argv )
      {
	 int flag;
	 MPI_Initialized(&flag);
	 if(!flag)
	    MPI_INSIST(MPI_Init(&argc, &argv));

	 if( !initialized )
         {
	    // Need to initialise data types here because we calculate sizes during construction, which
	    // needs MPI to be initialized. Note that NULL is already done.
	    mpi::data_type::byte.mpi_data_type(MPI_BYTE);
#if defined( MPICH ) || defined( MPICH2 )
	    mpi::data_type::boolean.mpi_data_type(MPIR_CXX_BOOL);
#else
	    mpi::data_type::boolean.mpi_data_type(MPI_C_BOOL);
#endif
	    mpi::data_type::character.mpi_data_type(MPI_CHAR);
	    mpi::data_type::integer.mpi_data_type(MPI_INT);
	    mpi::data_type::unsigned_integer.mpi_data_type(MPI_UNSIGNED);
	    mpi::data_type::long_integer.mpi_data_type(MPI_LONG);
	    mpi::data_type::unsigned_long.mpi_data_type(MPI_UNSIGNED_LONG);
	    mpi::data_type::long_long.mpi_data_type(MPI_LONG_LONG);
	    mpi::data_type::unsigned_long_long.mpi_data_type(MPI_UNSIGNED_LONG_LONG);
	    mpi::data_type::floating.mpi_data_type(MPI_FLOAT);
	    mpi::data_type::double_floating.mpi_data_type(MPI_DOUBLE);

	    // Advance shared pointer counts.
	    ++_shared_ptr_cnts.set_default(&mpi::comm::null, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::comm::self, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::comm::world, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::null, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::byte, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::boolean, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::character, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::integer, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::unsigned_integer, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::long_integer, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::unsigned_long, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::long_long, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::unsigned_long_long, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::floating, 0)->second;
	    ++_shared_ptr_cnts.set_default(&mpi::data_type::double_floating, 0)->second;

            // If we are not using MPICH2 (i.e. OpenMPI) update the
            // data type mapping.
#if !( defined( MPICH ) || defined( MPICH2 ) )
            mpi::data_type::_type_map[0] = MPI_BYTE;
            mpi::data_type::_type_map[1] = MPI_C_BOOL;
            mpi::data_type::_type_map[2] = MPI_CHAR;
            mpi::data_type::_type_map[3] = MPI_INT;
            mpi::data_type::_type_map[4] = MPI_UNSIGNED;
            mpi::data_type::_type_map[5] = MPI_LONG;
            mpi::data_type::_type_map[6] = MPI_UNSIGNED_LONG;
            mpi::data_type::_type_map[7] = MPI_LONG_LONG;
            mpi::data_type::_type_map[8] = MPI_UNSIGNED_LONG_LONG;
            mpi::data_type::_type_map[9] = MPI_FLOAT;
            mpi::data_type::_type_map[10] = MPI_DOUBLE;
#endif

	    initialized = true;
	 }

#ifndef NLOG
         // If this is the first time I'm initialising MPI, broadcast a logging
         // base time to use.
         log_base_time = MPI_Wtime();
         mpi::comm::world.bcast( log_base_time, 0 );
#endif
      }

      void finalise(bool mpi) {
	 if(!initialized) {

	    // Eliminate shared pointer counts.
	    _shared_ptr_cnts.erase(&mpi::comm::null);
	    _shared_ptr_cnts.erase(&mpi::comm::self);
	    _shared_ptr_cnts.erase(&mpi::comm::world);
	    _shared_ptr_cnts.erase(&mpi::data_type::null);
	    _shared_ptr_cnts.erase(&mpi::data_type::byte);
	    _shared_ptr_cnts.erase(&mpi::data_type::boolean);
	    _shared_ptr_cnts.erase(&mpi::data_type::character);
	    _shared_ptr_cnts.erase(&mpi::data_type::integer);
	    _shared_ptr_cnts.erase(&mpi::data_type::unsigned_integer);
	    _shared_ptr_cnts.erase(&mpi::data_type::long_integer);
	    _shared_ptr_cnts.erase(&mpi::data_type::unsigned_long);
	    _shared_ptr_cnts.erase(&mpi::data_type::long_long);
	    _shared_ptr_cnts.erase(&mpi::data_type::unsigned_long_long);
	    _shared_ptr_cnts.erase(&mpi::data_type::floating);
	    _shared_ptr_cnts.erase(&mpi::data_type::double_floating);

	    initialized = false;
	 }

	 if( mpi ) {
	    int flag;
	    MPI_Initialized(&flag);
	    if(flag)
	       MPI_INSIST(MPI_Finalize());
	 }
      }
   }
}
