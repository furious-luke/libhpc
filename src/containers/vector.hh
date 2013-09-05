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

#ifndef libhpc_containers_vector_hh
#define libhpc_containers_vector_hh

#include "std_vector.hh"
#include "vector_view.hh"
#include "global.hh"

namespace hpc {

   template< class T >
   class vector
      : public impl::std::vector< T >
   {
   public:

      typedef index key_type;
      typedef T mapped_type;

      vector()
	 : impl::std::vector<T>()
      {
      }

      explicit vector( typename impl::std::vector<T>::size_type size )
	 : impl::std::vector<T>(size)
      {
      }

      vector( const vector<T>& src )
	 : impl::std::vector<T>( src )
      {
      }

      vector( vector<T>&& src )
	 : impl::std::vector<T>( src )
      {
      }

      vector( const typename vector<T>::view& src )
	 : impl::std::vector<T>()
      {
	 this->duplicate(src);
      }

      template< class Iter >
      vector( Iter first,
	      Iter last )
	 : impl::std::vector<T>(first, last)
      {
      }

      ~vector()
      {
      }

      void
      set_range( T end,
		 T begin=0 )
      {
	 ASSERT(end >= begin, "Invalid range.");
	 T size = end - begin;
	 this->resize(size);
	 for(T ii = 0; ii < size; ++ii)
	    (*this)[ii] = ii;
      }

      void
      duplicate( const typename vector<T>::view& src )
      {
	 this->reallocate(src.size());
	 ::std::copy(src.begin(), src.end(), this->begin());
      }

      void
      clone( const vector<T>& src )
      {
	 this->reallocate( src.capacity() );
         this->resize( src.size() );
	 ::std::copy( src.begin(), src.end(), this->begin() );
      }

      vector<T>&
      operator=( const vector<T>& op )
      {
      	 return (vector<T>&)impl::std::vector<T>::operator=( op );
      }

      vector<T>&
      operator=( vector<T>&& op )
      {
         return (vector<T>&)impl::std::vector<T>::operator=( std::move( op ) );
      }

      vector<T>&
      operator=( const typename vector<T>::view& op )
      {
      	 this->resize( op.size() );
      	 ::std::copy( op.begin(), op.end(), this->begin() );
         return *this;
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const vector<T>& obj )
      {
	 strm << "[";
	 if(obj.size()) {
	    strm << obj[0];
	    for(index ii = 1; ii < obj.size(); ++ii)
	       strm << ", " << obj[ii];
	 }
	 strm << "]";
	 return strm;
      }
   };
};

#endif
