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

#ifndef libhpc_containers_vector_view_hh
#define libhpc_containers_vector_view_hh

#include "libhpc/debug/assert.hh"
#include "libhpc/system/types.hh"
#include "libhpc/system/type_traits.hh"

namespace hpc {

   template< class Vector >
   class vector_view {
   public:

      typedef index size_type;
      typedef index key_type;
      typedef typename Vector::value_type mapped_type;
      typedef typename Vector::value_type value_type;
      typedef typename Vector::const_pointer const_pointer;
      typedef typename Vector::pointer pointer;
      typedef typename Vector::const_reference const_element_reference;
      typedef typename Vector::reference element_reference;
      typedef typename Vector::iterator iterator;
      typedef typename Vector::const_iterator const_iterator;

      vector_view();

      vector_view(const Vector& vec);

      vector_view(const Vector& vec,
		  index size,
		  index offs=0);

      vector_view( const vector_view& src );

      vector_view( const vector_view& view,
		   index size,
		   index offs=0 )
      {
	 ASSERT(size >= 0);
	 ASSERT(offs >= 0);
	 ASSERT(offs + size <= view.size());
	 this->_ptr = (pointer)view.data() + offs;
	 this->_size = size;
      }

      vector_view( const_pointer ptr,
                   index size,
                   index offs=0 )
      {
         this->_ptr = (pointer)ptr + offs;
         this->_size = size;
      }

      ~vector_view();

      void
      setup( const vector_view& src )
      {
	 this->_size = src._size;
	 this->_ptr = src._ptr;
      }

      void
      setup( const vector_view& src,
	     index size,
	     index offs=0 )
      {
	 ASSERT(size >= 0);
	 ASSERT(offs >= 0);
	 ASSERT(offs + size <= src.size());
	 this->_ptr = (pointer)src.data() + offs;
	 this->_size = size;
      }

      void
      shrink( index size,
              index offs = 0 )
      {
         ASSERT( size + offs <= this->_size );
         this->_ptr += offs;
         this->_size = size;
      }

      size_type
      size() const;

      bool
      empty() const;

      const pointer
      data() const;

      const_iterator
      begin() const;

      iterator
      begin();

      const_iterator
      end() const;

      iterator
      end();

      const_element_reference
      front() const
      {
	 return *this->begin();
      }

      const_element_reference
      back() const
      {
	 return *(this->end() - 1);
      }

      const_element_reference
      operator[](int idx) const;

      element_reference
      operator[](int idx);

      vector_view&
      operator=( const vector_view& src )
      {
	 ::std::copy(src.begin(), src.end(), this->begin());
      }

      bool
      operator==(const vector_view& op) const;

      operator pointer()
      {
	 return this->_ptr;
      }

      operator const pointer() const
      {
	 return this->_ptr;
      }

      friend std::ostream&
      operator<<( std::ostream& strm,
		  const vector_view<Vector>& obj )
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

   private:
      pointer _ptr;
      size_type _size;
   };


   template< class Vector >
   struct type_traits< vector_view<Vector> >
   {
      typedef vector_view<Vector> value;
      typedef const vector_view<Vector> const_value;
      typedef vector_view<Vector> reference;
      typedef const vector_view<Vector> const_reference;
   };
}

#include "vector_view.tcc"

#endif
