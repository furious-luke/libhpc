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
#include "datatype.hh"
#include "comm.hh"
#include "insist.hh"
#include "logger.hh"

namespace hpc {
   namespace mpi {

      bool _init = false;

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
         // static std::shared_ptr<mpi::comm> static_comms[3];
         // static std::shared_ptr<mpi::datatype> static_types[3];

	 int flag;
	 MPI_Initialized( &flag );
	 if( !flag )
	    MPI_INSIST( MPI_Init( &argc, &argv ) );

	 if( !_init )
         {
	    // Need to initialise data types here because we calculate sizes during construction, which
	    // needs MPI to be initialized. Note that NULL is already done.
	    mpi::datatype::byte.mpi_datatype(               MPI_BYTE );
#if defined( MPICH ) || defined( MPICH2 )
	    mpi::datatype::boolean.mpi_datatype(            MPIR_CXX_BOOL );
#elif OMPI_MAJOR_VERSION <= 1 || (OMPI_MAJOR_VERSION == 1 && OMPI_MINOR_VERSION <= 4)
            mpi::datatype::boolean.mpi_datatype(            MPI_CHAR );
#else
	    mpi::datatype::boolean.mpi_datatype(            MPI_C_BOOL );
#endif
	    mpi::datatype::character.mpi_datatype(          MPI_CHAR );
	    mpi::datatype::integer.mpi_datatype(            MPI_INT );
	    mpi::datatype::unsigned_integer.mpi_datatype(   MPI_UNSIGNED );
	    mpi::datatype::long_integer.mpi_datatype(       MPI_LONG );
	    mpi::datatype::unsigned_long.mpi_datatype(      MPI_UNSIGNED_LONG );
	    mpi::datatype::long_long.mpi_datatype(          MPI_LONG_LONG );
	    mpi::datatype::unsigned_long_long.mpi_datatype( MPI_UNSIGNED_LONG_LONG );
	    mpi::datatype::floating.mpi_datatype(           MPI_FLOAT );
	    mpi::datatype::double_floating.mpi_datatype(    MPI_DOUBLE );

            // // Prepare static types.
            // static_comms[0].reset( &mpi::comm::null );
            // static_comms[1].reset( &mpi::comm::self );
            // static_comms[2].reset( &mpi::comm::world );
            // static_types[0].reset( &mpi::datatype::null );
            // static_types[1].reset( &mpi::datatype::byte );
            // static_types[2].reset( &mpi::datatype::boolean );
            // static_types[3].reset( &mpi::datatype::character );
            // static_types[4].reset( &mpi::datatype::integer );
            // static_types[5].reset( &mpi::datatype::unsigned_integer );
            // static_types[6].reset( &mpi::datatype::long_integer );
            // static_types[7].reset( &mpi::datatype::unsigned_long );
            // static_types[8].reset( &mpi::datatype::long_long );
            // static_types[9].reset( &mpi::datatype::unsigned_long_long );
            // static_types[10].reset( &mpi::datatype::floating );
            // static_types[11].reset( &mpi::datatype::double_floating );

            // If we are not using MPICH2 (i.e. OpenMPI) update the
            // data type mapping.
#if !( defined( MPICH ) || defined( MPICH2 ) )
            mpi::_type_map[0]  = MPI_BYTE;
#if OMPI_MAJOR_VERSION <= 1 || (OMPI_MAJOR_VERSION == 1 && OMPI_MINOR_VERSION <= 4)
            mpi::_type_map[1]  = MPI_CHAR;
#else
            mpi::_type_map[1]  = MPI_C_BOOL;
#endif
            mpi::_type_map[2]  = MPI_CHAR;
            mpi::_type_map[3]  = MPI_INT;
            mpi::_type_map[4]  = MPI_UNSIGNED;
            mpi::_type_map[5]  = MPI_LONG;
            mpi::_type_map[6]  = MPI_UNSIGNED_LONG;
            mpi::_type_map[7]  = MPI_LONG_LONG;
            mpi::_type_map[8]  = MPI_UNSIGNED_LONG_LONG;
            mpi::_type_map[9]  = MPI_FLOAT;
            mpi::_type_map[10] = MPI_DOUBLE;
#endif

	    _init = true;
	 }

#ifndef NLOG
         // If this is the first time I'm initialising MPI, broadcast a logging
         // base time to use.
         log_base_time = MPI_Wtime();
         mpi::comm::world.bcast( log_base_time, 0 );
#endif
      }

      bool
      initialised()
      {
	 int flag;
	 MPI_Initialized( &flag );
         return flag;
      }

      void finalise( bool mpi )
      {
	 if( !_init )
         {
	    // // Eliminate shared pointer counts.
	    // _shared_ptr_cnts.erase(&mpi::comm::null);
	    // _shared_ptr_cnts.erase(&mpi::comm::self);
	    // _shared_ptr_cnts.erase(&mpi::comm::world);
	    // _shared_ptr_cnts.erase(&mpi::datatype::null);
	    // _shared_ptr_cnts.erase(&mpi::datatype::byte);
	    // _shared_ptr_cnts.erase(&mpi::datatype::boolean);
	    // _shared_ptr_cnts.erase(&mpi::datatype::character);
	    // _shared_ptr_cnts.erase(&mpi::datatype::integer);
	    // _shared_ptr_cnts.erase(&mpi::datatype::unsigned_integer);
	    // _shared_ptr_cnts.erase(&mpi::datatype::long_integer);
	    // _shared_ptr_cnts.erase(&mpi::datatype::unsigned_long);
	    // _shared_ptr_cnts.erase(&mpi::datatype::long_long);
	    // _shared_ptr_cnts.erase(&mpi::datatype::unsigned_long_long);
	    // _shared_ptr_cnts.erase(&mpi::datatype::floating);
	    // _shared_ptr_cnts.erase(&mpi::datatype::double_floating);

	    _init = false;
	 }

	 if( mpi )
         {
	    int flag;
	    MPI_Initialized( &flag );
	    if( flag )
	       MPI_INSIST( MPI_Finalize() );
	 }
      }

   }
}
