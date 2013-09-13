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

#ifndef hpc_algorithm_farmer_hh
#define hpc_algorithm_farmer_hh

#include "libhpc/debug/assert.hh"
#include "libhpc/logging/logging.hh"
#include "libhpc/mpi/comm.hh"

namespace hpc {
   namespace algorithm {

      template< class Master,
		class Worker >
      class farmer
      {
      public:

         typedef Master master_type;
         typedef Worker worker_type;

      public:

         farmer( const mpi::comm& comm = mpi::comm::world,
                 int tag = 8503 )
            : _own_mast( true ),
              _own_work( true ),
              _mast( 0 ),
              _work( 0 ),
              _comm( &comm ),
              _max_its( 10 ),
              _tag( tag ),
              _comm( 0 )
         {
         }

         ~farmer()
         {
            if( _own_mast && _mast )
               delete _mast;
            if( _own_work && _work )
               delete _work;
         }

         void
         set_comm( const mpi::comm& comm )
         {
            _comm = &comm;
            if( _mast )
               _mast->set_comm( comm );
            if( _work )
               _work->set_comm( comm );
         }

         void
         set_master( master_type* mast )
         {
            if( _own_mast && _mast )
               delete _mast;
            _mast = mast;
            _own_mast == (bool)_mast;
         }

         void
         set_worker( worker_type* work )
         {
            if( _own_work && _work )
               delete _work;
            _work = work;
            _own_work == (bool)_work;
         }

         void
         run()
         {
            // Do something different if we're operating in serial.
            if( _comm->size() == 1 )
               _serial();
            else if( _comm->rank() == 0 )
               _master();
            else
               _worker();
         }

      protected:

         void
         _serial()
         {
            LOGDLN( "Entering serial loop.", setindent( 1 ) );

            // Construct.
            if( !_mast )
               _mast = new master_type;
            if( !_work )
               _work = new worker_type;

            // Loop until all complete.
            bool done = false;
            while( !done )
            {
               // Do we have anything remaining?
               if( _first != _last )
               {
                  // Call worker to process.
                  range<unsigned long long> rng({ _first, _last });
                  _work->process( rng );

                  // Update for next batch.
                  _mast->next( *this, _first, _last );
               }
               else
                  done = true;

               // Call out to any idling process.
               _mast->idle( *this );
            }

            LOGDLN( "Done.", setindent( -1 ) );
         }

         void
         _master()
         {
            LOGDLN( "Entering farming master.", setindent( 1 ) );

            // Construct.
            if( !_mast )
               _mast = new master_type;

            // Prepare the first batch.
            _mast->next( _first, _last );

            // Set remaining ranks.
            _rem = _comm->size() - 1;

            // Loop until all complete.
            bool done = false;
            while( !done )
            {
               // Wait for requests for more.
               MPI_Status stat;
               unsigned it = 0;
               while( it++ < _max_its && _comm->iprobe( stat, MPI_ANY_SOURCE, _tag ) )
               {
                  LOGDLN( "Have an incoming request from rank: ", stat.MPI_SOURCE );

                  int flag;
                  _comm->recv<int>( flag, stat.MPI_SOURCE, _tag );

                  // Do we have anything remaining?
                  vector<unsigned long long> rng( 2 );
                  if( _first != _last )
                  {
                     // Send out new batch.
                     rng[0] = _first;
                     rng[1] = _last;
                     LOGDLN( "Sending out range: [", rng[0], "-", rng[1], ")" );
                     _comm->send<unsigned long long>( rng, stat.MPI_SOURCE, _tag );
                     _first = _last;

                     // Update for next batch.
                     _mast->next( *this, _first, _last );
                  }
                  else
                  {
                     // Reply indicating empty range.
                     rng[0] = rng[1] = 0;
                     _comm->send<unsigned long long>( rng, stat.MPI_SOURCE, _tag );

                     // Decrement remaining ranks.
                     --_rem;
                     LOGDLN( "Signalling termination, ", _rem, " remaining ranks." );
                  }
               }

               // Call out to any idling process.
               _mast->idle( *this );

               done = (_rem == 0);
            }

            LOGDLN( "Done.", setindent( -1 ) );
         }

         void
         _worker()
         {
            LOGDLN( "Entering worker loop.", setindent( 1 ) );

            // Construct.
            if( !_work )
               _work = new worker_type;

            // Loop until we're done.
            bool done = false;
            while( !done )
            {
               // Request next allocation.
               LOGDLN( "Sending request for tasks." );
               int flag = 1;
               _comm->send<int>( flag, 0, _tag );

               // Receive new task range.
              vector<unsigned long long> rng( 2 );
               _comm->recv<unsigned long long>( rng, 0, _tag );
               LOGDLN( "Received task range: [", rng[0], "-", rng[1], ")" );

               // If the range is empty it means we're done.
               range<unsigned long long> rng( rng[0], rng[1] );
               if( rng.empty() )
                  done = true;
               else
                  _work->process( rng );
            }

            LOGDLN( "Done.", setindent( -1 ) );
         }

      protected:

         bool _own_mast;
         bool _own_work;
         master_type* _mast;
         worker_type* _work;
         unsigned long long _first, _last;
         unsigned _rem;
         unsigned _max_its;
         int _tag;
         const mpi::comm* _comm;
      };

   }
}

#endif
