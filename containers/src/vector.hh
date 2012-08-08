#ifndef containers_vector_hh
#define containers_vector_hh

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
      	 impl::std::vector<T>::operator=(op);
      }

      vector<T>&
      operator=( const typename vector<T>::view& op )
      {
      	 this->resize(op.size());
      	 ::std::copy(op.begin(), op.end(), this->begin());
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
