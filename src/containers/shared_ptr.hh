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

#ifndef containers_shared_ptr_hh
#define containers_shared_ptr_hh

#include "libhpc/debug/assert.hh"
#include "map.hh"

namespace hpc {

   extern map<void*, int> _shared_ptr_cnts;

   template< class T >
   class shared_ptr
   {
   public:

      shared_ptr( const T* ptr=NULL )
	 : _ptr((T*)ptr)
      {
	 ++(*this);
      }

      shared_ptr( const T& obj )
	 : _ptr((T*)&obj)
      {
	 ++(*this);
      }

      shared_ptr( const shared_ptr& src )
	 : _ptr((T*)src._ptr)
      {
	 ++(*this);
      }

      ~shared_ptr()
      {
	 --(*this);
      }

      void
      reset()
      {
	 --(*this);
	 this->_ptr = NULL;
      }

      bool
      empty() const
      {
	 return this->_ptr == NULL;
      }

      T* get()
      {
         return _ptr;
      }

      shared_ptr&
      operator=( const T& op )
      {
	 --(*this);
	 this->_ptr = (T*)&op;
	 ++(*this);
	 return *this;
      }

      shared_ptr&
      operator=( const T* op )
      {
	 --(*this);
	 this->_ptr = (T*)op;
	 ++(*this);
	 return *this;
      }

      shared_ptr&
      operator=( const shared_ptr& op )
      {
	 --(*this);
	 this->_ptr = (T*)op._ptr;
	 ++(*this);
	 return *this;
      }

      void
      operator++()
      {
	 if(this->_ptr)
	    ++_shared_ptr_cnts.set_default((void*)this->_ptr, 0)->second;
      }

      void
      operator--()
      {
	 if(this->_ptr) {
	    map<void*, int>::iterator it = _shared_ptr_cnts.find((void*)this->_ptr);
	    ASSERT(it->second > 0);
	    if(!--it->second) {
	       _shared_ptr_cnts.erase(it);
	       delete this->_ptr;
	       this->_ptr = NULL;
	    }
	 }
      }

      bool
      operator==( const shared_ptr& op )
      {
	 return this->_ptr == op._ptr;
      }

      bool
      operator==( const T* op )
      {
	 return this->_ptr == op;
      }

      bool
      operator!=( const shared_ptr& op )
      {
	 return this->_ptr != op._ptr;
      }

      bool
      operator!=( const T* op )
      {
	 return this->_ptr != op;
      }

      const T&
      operator*() const
      {
	 ASSERT(this->_ptr != NULL);
	 return *this->_ptr;
      }

      T&
      operator*()
      {
	 ASSERT(this->_ptr != NULL);
	 return *this->_ptr;
      }

      const T*
      operator->() const
      {
	 return this->_ptr;
      }

      T*
      operator->()
      {
	 return this->_ptr;
      }

      operator const T*() const
      {
	 return this->_ptr;
      }

      operator T*()
      {
	 return this->_ptr;
      }

   private:
      T* _ptr;
   };
}

#endif
