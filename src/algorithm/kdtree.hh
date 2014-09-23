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

#ifndef hpc_algorithm_kdtree_hh
#define hpc_algorithm_kdtree_hh

#include <boost/range/algorithm/copy.hpp>

namespace hpc {

   template< class CoordT = double >
   class kdtree
   {
   public:

      typedef CoordT coord_type;

      struct split_type
      {
         coord_type pos;
         unsigned dim;
      };

   public:

      kdtree( mpi::comm const& comm = mpi::comm::world )
         : _comm( &comm )
      {
      }

      template< class PartT >
      kdtree( PartT& part,
              mpi::comm const& comm = mpi::comm::world )
         : _comm( &comm )
      {
         construct( part );
      }

      void
      clear()
      {
         hpc::deallocate( _bnds );
	 hpc::deallocate( _root_splits );
	 hpc::deallocate( _splits );
      }

      template< class PartT >
      void
      construct( PartT& part )
      {
         LOGBLOCKD( "Constructing kdtree." );

	 clear();

         // Get initial bounds and resize for the local splits.
         _bnds = part.bounds();
	 _splits.resize( part.n_branch_cells() );
         LOGDLN( "Initial bounds: ", _bnds );
         LOGDLN( "Leaf cells:   ", n_leafs() );
         LOGDLN( "Branch cells: ", part.n_branch_cells() );
         LOGDLN( "Total cells:  ", n_cells() );

	 // Perform parallel part of partitioning.
         std::vector<std::array<coord_type,2> > bnds = _bnds;
	 {
            LOGBLOCKD( "Performing parallel refinement." );

	    std::list<std::tuple<coord_type,unsigned,int> > splits;
	    mpi::comm comm( *_comm );

            // Refine until we hit serial or finish splitting.
	    while( comm.size() != 1 )
	    {
	       LOGBLOCKD( "Bounds: ", bnds );
	       part.partition( 0, bnds, comm );
	       comm = part.sub_comm();
               auto split = part.split_and_side();
	       LOGDLN( "Split: ", std::get<0>( split ), ", ", std::get<1>( split ) );
	       LOGDLN( "Side:  ", std::get<2>( split ) );
	       splits.push_back( split );
               if( std::get<2>( split ) == 0 )
                  std::get<1>( bnds[std::get<1>( split )] ) = std::get<0>( split );
               else
                  std::get<0>( bnds[std::get<1>( split )] ) = std::get<0>( split );

	       // Update local boundary.
	       _bnds[std::get<1>( split )][std::get<2>( split ) ? 0 : 1] = std::get<0>( split );
	       LOGDLN( "New bounds: ", _bnds );
	    }

            // Transfer
	    _root_splits.resize( splits.size() );
	    boost::copy( splits, _root_splits.begin() );
	 }

	 // Now continue partitioning serially. We need to
         // avoid this if we are already complete.
         {
            LOGBLOCKD( "Performing serial refinement." );

            if( part.need_refinement( 0 ) )
               _construct_serial_recursive( part, 0, bnds );
         }
      }

      template< class PartT >
      void
      _construct_serial_recursive( PartT& part,
                                   unsigned cell,
                                   std::vector<std::array<coord_type,2> >& bnds )
      {
         LOGBLOCKD( "Refining cell: ", cell );
         LOGDLN( "Bounds: ", bnds );

         part.partition( cell, bnds, mpi::comm::self );
         _splits[cell].pos = std::get<0>( part.split() );
         _splits[cell].dim = std::get<1>( part.split() );
         LOGDLN( "Split: ", _splits[cell].pos, ", ", _splits[cell].dim );

         if( part.need_refinement( part.left_child( cell ) ) )
         {
            coord_type tmp = bnds[_splits[cell].dim][1];
            bnds[_splits[cell].dim][1] = _splits[cell].pos;
            _construct_serial_recursive( part, part.left_child( cell ), bnds );
            bnds[_splits[cell].dim][1] = tmp;
         }
         if( part.need_refinement( part.right_child( cell ) ) )
         {
            coord_type tmp = bnds[_splits[cell].dim][0];
            bnds[_splits[cell].dim][0] = _splits[cell].pos;
            _construct_serial_recursive( part, part.right_child( cell ), bnds );
            bnds[_splits[cell].dim][0] = tmp;
         }
      }

      // void
      // n_cells() const
      // {
      //    return _root_splits.size() + _splits.size();
      // }

      // bool
      // has_left_child( unsigned cell ) const
      // {
      //    if( cell < _root_splits.size() )
      //       return std::get<2>( _root_splits[cell] ) == 0;
      //    else
      //       return cell < ;
      // }

      // unsigned
      // left_child( unsigned cell ) const
      // {
      //    return (cell + 1)*2;
      // }

      // void
      // right_child( unsigned cell ) const
      // {
      //    return _left_child( cell ) + 1;
      // }

      unsigned
      n_dims() const
      {
         return _bnds.size();
      }

      unsigned
      n_leafs() const
      {
         return _splits.size() + 1;
      }

      unsigned
      n_branches() const
      {
         return _splits.size();
      }

      unsigned
      n_cells() const
      {
         return n_leafs() + n_branches();
      }

      mpi::comm const&
      comm() const
      {
         return *_comm;
      }

      std::vector<std::array<coord_type,2> > const&
      bounds() const
      {
         return _bnds;
      }

      std::vector<split_type> const&
      splits() const
      {
         return _splits;
      }

      template< class IterT >
      void
      leaf_geometry( unsigned leaf,
                     IterT result ) const
      {
         unsigned cell = leaf_to_cell( leaf );
         std::vector<bool> done( 2*_bnds.size() );
         std::fill( done.begin(), done.end(), false );
         while( cell > 0 )
         {
            auto split = _splits[_parent( cell )];
            unsigned idx = 2*split.dim;
            if( (cell & 1) != 0 )
               ++idx;
            if( !done[idx] )
            {
               *(result + idx) = split.pos;
               done[idx] = true;
            }
            cell = _parent( cell );
         }
         for( unsigned ii = 0; ii < done.size(); ++ii )
         {
            if( !done[ii] )
               *(result + ii) = _bnds[ii >> 1][ii & 1];
         }
      }

      unsigned
      leaf_to_cell( unsigned leaf ) const
      {
         unsigned md = _max_depth();
         unsigned cell;
         if( md )
         {
            unsigned nil = _n_inc_leafs();
            if( leaf < nil )
               cell = leaf + n_cells() - nil;
            else
               cell = n_cells() - nil - (1 << (md - 1)) + (leaf - nil) + (nil >> 1);
         }
         else
            cell = 0;
         return cell;
      }

   protected:

      unsigned
      _max_depth() const
      {
         unsigned nl = n_leafs();
         unsigned dep = log2i( nl );
         if( (1 << dep) < nl )
            ++dep;
         return dep;
      }

      unsigned
      _n_inc_leafs() const
      {
         unsigned md = _max_depth();
         if( md )
            return 2*(n_leafs() - (1 << (md - 1)));
         else
            return 0;
      }

      unsigned
      _parent( unsigned cell ) const
      {
         ASSERT( cell > 0 );
         return (cell - 1) >> 1;
      }

   protected:

      std::vector<std::array<coord_type,2> > _bnds;
      std::vector<std::tuple<coord_type,unsigned,int> > _root_splits;
      std::vector<split_type> _splits;
      mpi::comm const* _comm;
   };

}

#endif
