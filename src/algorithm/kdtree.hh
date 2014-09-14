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

namespace hpc {

   template< class CoordT = double >
   class kdtree
   {
   public:

      typedef CoordT coord_type;

   public:

      kdtree();

      void
      clear()
      {
	 hpc::deallocate( _splits );
      }

      template< class PartT >
      void
      construct( PartT& partition )
      {
	 clear();

	 _min = partition.min();
	 _max = partition.max();
	 _splits.resize( partition.n_local_cells() );

	 n_cells = _parts.size();
	 max_depth = log2<unsigned>( n_cells );
	 if( 1 << max_depth != n_cells )
	 {
	    n_inc_cells = n_cells - (1 << max_depth);
	    ++max_depth;
	 }
	 else
	    n_inc_cells = 0;

	 // Perform parallel part of partitioning.
	 {
	    std::list<std::tuple<coord_type,unsigned,int> > splits;
	    mpi::comm comm( *_comm );
	    while( comm.size() != 1 && !partition.done() )
	    {
	       partition.partition( *this, 0, comm );
	       comm = partition.comm();
	       splits.push_back( partition.split_and_side() );
	    }
	    _root_splits.resize( splits.size() );
	    boost::copy( splits, _root_splits.begin() );
	 }

	 // Now continue partitioning serially.
	 unsigned ii = 0;
	 while( !partition.done() )
	 {
	    partition.partition( *this, ii, mpi::comm::self );
	    _splits[ii] = partition.split();
	    ++ii;
	 }
b      }

      void
      n_cells() const
      {
	 return _root_splits.size() + _splits.size();
      }

      bool
      has_left_child( unsigned cell ) const
      {
	 if( cell < _root_splits.size() )
	    return std::get<2>( _root_splits[cell] ) == 0;
	 else
	    return cell < 

      unsigned
      left_child( unsigned cell ) const
      {
	 return (cell + 1)*2;
      }

      void
      right_child( unsigned cell ) const
      {
	 return _left_child( cell ) + 1;
      }

   protected:

      std::vector<coord_type> _min, _max;
      std::list<std::tuple<coord_type,unsigned,int> > _root_splits;
      std::vector<std::tuple<coord_type,unsigned> > _splits;
   };

}

#endif
