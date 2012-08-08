#ifndef containers_vector_view_tcc
#define containers_vector_view_tcc

#include "libhpc/debug/assert.hh"

namespace hpc {

  template<class Vector>
  vector_view<Vector>::vector_view()
    : _ptr(NULL),
      _size(0)
  {
  }

  template<class Vector>
  vector_view<Vector>::vector_view(const Vector& vec)
    : _ptr((pointer)vec.data()),
      _size(vec.size())
  {
  }

  template<class Vector>
  vector_view<Vector>::vector_view(const Vector& vec,
				   index size,
				   index offs)
  {
    ASSERT(size >= 0);
    ASSERT(offs >= 0);
    ASSERT(offs + size <= vec.size());
    this->_ptr = (pointer)vec.data() + offs;
    this->_size = size;
  }

  template<class Vector>
  vector_view<Vector>::vector_view(const vector_view& src)
    : _ptr(src._ptr),
      _size(src._size)
  {
  }

  template<class Vector>
  vector_view<Vector>::~vector_view()
  {
  }

  template<class Vector>
  typename vector_view<Vector>::size_type
  vector_view<Vector>::size() const
  {
    return this->_size;
  }

  template<class Vector>
  bool
  vector_view<Vector>::empty() const
  {
    return this->_size == 0;
  }

  template<class Vector>
  const typename vector_view<Vector>::pointer
  vector_view<Vector>::data() const
  {
    return this->_ptr;
  }

  template<class Vector>
  typename Vector::const_reference
  vector_view<Vector>::operator[](int idx) const
  {
    ASSERT(idx >= 0 && idx < this->size(), "Index out of bounds.");
    return this->_ptr[idx];
  }

  template<class Vector>
  typename Vector::reference
  vector_view<Vector>::operator[](int idx)
  {
    ASSERT(idx >= 0 && idx < this->size(), "Index out of bounds.");
    return this->_ptr[idx];
  }

  template<class Vector>
  bool
  vector_view<Vector>::operator==(const vector_view& op) const
  {
    if(this->_size != op._size)
      return false;
    for(int ii = 0; ii < this->_size; ++ii) {
      if(this->_ptr[ii] != op._ptr[ii])
	return false;
    }
    return true;
  }

  template<class Vector>
  typename vector_view<Vector>::const_iterator
  vector_view<Vector>::begin() const
  {
    return const_iterator(this->_ptr);
  }

  template<class Vector>
  typename vector_view<Vector>::iterator
  vector_view<Vector>::begin()
  {
    return iterator(this->_ptr);
  }

  template<class Vector>
  typename vector_view<Vector>::const_iterator
  vector_view<Vector>::end() const
  {
    return const_iterator(this->_ptr + this->_size);
  }

  template<class Vector>
  typename vector_view<Vector>::iterator
  vector_view<Vector>::end()
  {
    return iterator(this->_ptr + this->_size);
  }
}

#endif
