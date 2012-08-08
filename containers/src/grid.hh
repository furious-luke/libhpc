#ifndef hpc_containers_grid_hh
#define hpc_containers_grid_hh

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
      setup_block( unsigned dim,
                   T sides )
      {
         this->_sides.reallocate( dim );
         boost::fill( this->_sides, sides );
         this->_setup_basis();
      }

      template< class InputIterator >
      void
      setup( unsigned dim,
             InputIterator first )
      {
         this->_sides.reallocate( dim );
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
         for( unsigned ii = this->dimension(); ii > 0; --ii, --result ) {
            *result = index/this->_basis[ii - 1];
            index -= (*result)*this->_basis[ii - 1];
         }
      }

   protected:

      void
      _setup_basis()
      {
         this->_basis.reallocate( this->dimension() );
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
      vector<T> _sides;
      vector<T> _basis;
   };
}

#endif
