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

#ifndef libhpc_containers_grid_hh
#define libhpc_containers_grid_hh

#include <vector>
#include "libhpc/system/reallocate.hh"

namespace hpc {

   template< class T >
   class grid
   {
   public:

      grid()
         : _size( 0 )
      {
      }

      template< class InputIterator >
      grid( unsigned dim,
            InputIterator first )
      {
         this->setup( dim, first );
      }

      ~grid()
      {
      }

      void
      resize( unsigned dim,
              T sides )
      {
         setup_block( dim, sides );
      }

      void
      setup_block( unsigned dim,
                   T sides )
      {
         reallocate( _sides, dim );
         std::fill( _sides.begin(), _sides.end(), sides );
         this->_setup_basis();
      }

      template< class InputIterator >
      void
      setup( unsigned dim,
             InputIterator first )
      {
         reallocate( _sides, dim );
         for( unsigned ii = 0; ii < dim; ++ii, ++first )
            this->_sides[ii] = *first;
         this->_setup_basis();
      }

      unsigned
      dimension() const
      {
         return this->_sides.size();
      }

      T
      size() const
      {
         return this->_size;
      }

      template< class InputIterator >
      T
      project( InputIterator first )
      {
         T idx = 0;
         for( unsigned ii = 0; ii < this->dimension(); ++ii, ++first )
            idx += (*first)*this->_basis[ii];
         return idx;
      }

      template< class OutputIterator >
      void
      lift( T index,
            OutputIterator result )
      {
         result += this->dimension() - 1;
         for( unsigned ii = this->dimension(); ii > 0; --ii )
         {
            *result = index/this->_basis[ii - 1];
            index -= (*result)*this->_basis[ii - 1];

            // I put this here instead of in the loop description because I don't
            // want to decrement an iterator pointing to the first element. C++
            // debug mode complains about that sort of thing.
            if( ii > 1 )
               --result;
         }
      }

   protected:

      void
      _setup_basis()
      {
         reallocate( _basis, dimension() );
         if( this->_basis.size() ) {
            this->_size = this->_sides[0];
            this->_basis[0] = 1;
            for( unsigned ii = 1; ii < this->dimension(); ++ii ) {
               this->_size *= this->_sides[ii];
               this->_basis[ii] = this->_basis[ii - 1]*this->_sides[ii - 1];
            }
         }
      }

   private:

      T _size;
      std::vector<T> _sides;
      std::vector<T> _basis;
   };
}

#endif
