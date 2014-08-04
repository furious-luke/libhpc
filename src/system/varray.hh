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

#ifndef hpc_system_varray_hh
#define hpc_system_varray_hh

#include <iostream>
#include <boost/functional/hash.hpp>
#include "libhpc/system/cuda.hh"

namespace hpc {

   template< class T,
             std::size_t N >
   class varray
   {
   public:

      typedef T        value_type;
      typedef T*       iterator;
      typedef T const* const_iterator;

   public:

      T elems[N];

      const_iterator
      begin() const
      {
	 return elems;
      }

      iterator
      begin()
      {
	 return elems;
      }

      const_iterator
      end() const
      {
	 return elems + N;
      }

      iterator
      end()
      {
	 return elems + N;
      }

      CUDA_DEV_HOST
      T&
      operator[]( std::size_t idx )
      {
	 return elems[idx];
      }

      CUDA_DEV_HOST
      T const&
      operator[]( std::size_t idx ) const
      {
	 return elems[idx];
      }

      varray
      operator+( varray const& x ) const
      {
         varray z;
         for( int ii = 0; ii < N; ++ii )
            z[ii] = elems[ii] + x[ii];
         return z;
      }

      varray&
      operator+=( varray const& x )
      {
         for( int ii = 0; ii < N; ++ii )
            elems[ii] += x[ii];
         return *this;
      }

      varray
      operator/( value_type const& x ) const
      {
         varray y;
         for( int ii = 0; ii < N; ++ii )
            y[ii] = elems[ii]/x;
         return y;
      }

      value_type
      dot( varray const& x ) const
      {
         value_type y = 0.0;
         for( int ii = 0; ii < N; ++ii )
            y += elems[ii]*x[ii];
         return y;
      }

      value_type
      magnitude() const
      {
         value_type y = 0.0;
         for( int ii = 0; ii < N; ++ii )
            y += elems[ii]*elems[ii];
         return sqrt( y );
      }

      bool
      operator==( varray const& x ) const
      {
         for( int ii = 0; ii < N; ++ii )
         {
            if( elems[ii] != x[ii] )
               return false;
         }
         return true;
      }

      bool
      operator<( varray const& x ) const
      {
         for( int ii = N - 1; ii >= 0; --ii )
         {
            if( elems[ii] < x[ii] )
               return true;
            else if( elems[ii] > x[ii] )
               return false;
         }
         return false;
      }
   };

   template< class T,
             size_t N >
   std::ostream&
   operator<<( std::ostream& strm,
               hpc::varray<T,N> const& obj )
   {
      strm << "(";
      if( N )
      {
         typename hpc::varray<T,N>::const_iterator it = obj.begin();
         strm << *it++;
         while( it != obj.end() )
            strm << ", " << *it++;
      }
      strm << ")";
      return strm;
   }

}

namespace std {

   template< class T,
	     size_t N >
   struct hash< hpc::varray<T,N> >
   {
      size_t
      operator()( hpc::varray<T,N> const& x ) const
      {
	 size_t h = 0;
	 for( size_t ii = 0; ii < N; ++ii )
	    boost::hash_combine( h, boost::hash_value( x[ii] ) );
	 return h;
      }
   };

}

#endif
