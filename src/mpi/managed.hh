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

#ifndef hpc_mpi_managed_hh
#define hpc_mpi_managed_hh

#include "init.hh"
#include "comm.hh"
#include "async.hh"

namespace hpc {
   namespace mpi {

      template< class ManagerT,
                class WorkerT >
      class managed
      {
      public:

         typedef ManagerT manager_type;
         typedef WorkerT  worker_type;

      public:

         managed( mpi::comm const& comm = mpi::comm::world,
                  int mgr_rank = 0 )
            : _comm( &comm ),
              _mgr_rank( mgr_rank ),
              _mgr( 0 ),
              _wkr( 0 )
         {
         }

         mpi::comm const&
         comm() const
         {
            ASSERT( _comm );
            return *_comm;
         }

         int
         manager_rank() const
         {
            return _mgr_rank;
         }

         bool
         am_manager() const
         {
            return _mgr_rank == _comm->rank();
         }

         manager_type*
         manager()
         {
            return _mgr;
         }

         worker_type*
         worker()
         {
            return _wkr;
         }

         void
         process()
         {
            if( _comm->rank() == _mgr_rank )
            {
               if( !_mgr )
                  _mgr = new manager_type;
               _mgr->process();
            }
            else
            {
               if( !_wkr )
                  _wkr = new worker_type;
               _wkr->process();
            }
         }

      protected:

         int _mgr_rank;
         manager_type* _mgr;
         worker_type* _wkr;
         mpi::async _async;
         mpi::comm const* _comm;
      };

   }
}

#endif
