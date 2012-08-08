#ifndef containers_std_vector_tcc
#define containers_std_vector_tcc

namespace hpc {
  namespace impl {
    namespace std {

      template<class T>
      vector<T>::vector()
	: ::std::vector<T>()
      {
      }

      template<class T>
      vector<T>::vector(size_type size)
	: ::std::vector<T>(size)
      {
      }

      template<class T>
      vector<T>::~vector()
      {
      };

      template<class T>
      void
      vector<T>::deallocate()
      {
	this->clear();
	::std::vector<T>().swap(*this);
      }

      template<class T>
      void
      vector<T>::reallocate(size_type size)
      {
	this->deallocate();
	this->resize(size);
      }

      // template<class T>
      // void
      // vector<T>::duplicate(const vector<T>& src)
      // {
      //   this->reallocate(src.capacity());
      //   this->resize(src.size());
      //   ::std::copy(src.begin(), src.end(), this->begin());
      // }

      template<class T>
      void
      vector<T>::take(vector<T>& src)
      {
	this->swap(src);
	src.deallocate();
      }

      template<class T>
      typename vector<T>::const_reference
      vector<T>::operator[](size_type idx) const
      {
	ASSERT(idx >= 0 && idx < this->size(), "Index out of bounds.");
	return ::std::vector<T>::operator[](idx);
      }

      template<class T>
      typename vector<T>::reference
      vector<T>::operator[](size_type idx)
      {
	ASSERT(idx >= 0 && idx < this->size(), "Index out of bounds.");
	return ::std::vector<T>::operator[](idx);
      }
    }
  }
}

#endif
