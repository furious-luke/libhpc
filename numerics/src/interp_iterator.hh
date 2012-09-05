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

#ifndef libhpc_numerics_interp_iterator_hh
#define libhpc_numerics_interp_iterator_hh

#include <boost/iterator/iterator_facade.hpp>
#include "libhpc/debug/debug.hh"
#include "libhpc/containers/num.hh"
#include "libhpc/containers/vector.hh"

namespace hpc {

   template< class Grid0Iterator,
             class Grid1Iterator = Grid0Iterator,
             class Value = typename Grid0Iterator::value_type >
   class interp_iterator
      : public boost::iterator_facade< interp_iterator<Grid0Iterator, Grid1Iterator, Value>,
                                       Value,
                                       boost::forward_traversal_tag,
                                       Value,
                                       Value >
   {
   public:

      typedef Grid0Iterator grid0_iterator_type;
      typedef Grid1Iterator grid1_iterator_type;
      typedef Value value_type;

      interp_iterator()
      {
      }

      explicit interp_iterator( grid0_iterator_type grid0_start,
                                grid0_iterator_type grid0_finish,
                                grid1_iterator_type grid1_start,
                                grid1_iterator_type grid1_finish,
                                value_type epsilon = std::numeric_limits<value_type>::epsilon() )
         : _epsilon( epsilon )
      {
         _grid0_start = grid0_start;
         _grid0_finish = grid0_finish;
         _grid1_start = grid1_start;
         _grid1_finish = grid1_finish;
         _idxs[0] = _idxs[1] = 0;
         _select();
      }

      bool
      done() const
      {
         return _grid0_start == _grid0_finish && _grid1_start == _grid1_finish;
      }

      int
      side() const
      {
         return _side;
      }

      // TODO: Don't return a pointer!
      const size_t*
      indices() const
      {
         return _idxs;
      }

   protected:

      void
      increment()
      {
         if( _side == 0 )
         {
            ++_grid0_start;
            ++_idxs[0];
         }
         else if( _side == 1 )
         {
            ++_grid1_start;
            ++_idxs[1];
         }
         else
         {
            ++_grid0_start;
            ++_grid1_start;
            ++_idxs[0];
            ++_idxs[1];
         }
         _select();
      }

      value_type
      dereference() const
      {
         return (_side == 0) ? *_grid0_start : *_grid1_start;
      }

      bool
      equal( const interp_iterator& other ) const
      {
         ASSERT( 0 );
         return false;
      }

   protected:

      void
      _select()
      {
         if( _grid0_start == _grid0_finish )
            _side = 1;
         else if( _grid1_start == _grid1_finish )
            _side = 0;
         else if( num::approx<value_type>( *_grid0_start, *_grid1_start, _epsilon ) )
            _side = 2;
         else if( *_grid0_start < *_grid1_start )
            _side = 0;
         else
            _side = 1;
      }

   protected:

      int _side;
      size_t _idxs[2];
      grid0_iterator_type _grid0_start, _grid0_finish;
      grid1_iterator_type _grid1_start, _grid1_finish;
      value_type _epsilon;

      friend class boost::iterator_core_access;
   };

   template< class Grid0Iterator,
             class Grid1Iterator,
             class Value = typename Grid0Iterator::value_type >
   interp_iterator<Grid0Iterator, Grid1Iterator, Value>
   make_interp_iterator( Grid0Iterator grid0_start,
                         Grid0Iterator grid0_finish,
                         Grid1Iterator grid1_start,
                         Grid1Iterator grid1_finish,
                         Value epsilon = std::numeric_limits<Value>::epsilon() )
   {
      return interp_iterator<Grid0Iterator, Grid1Iterator, Value>(
         grid0_start, grid0_finish,
         grid1_start, grid1_finish,
         epsilon
         );
   }
}

#endif
