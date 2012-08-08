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

#ifndef debug_group_tcc
#define debug_group_tcc

#include <string.h>
#include "checks.hh"

namespace debug {

  template<class T>
  group<T>::group()
    : _left(-1),
      _right(-1),
      _select_idx(-1),
      _select_cnt(0)
  {
  }

  template<class T>
  group<T>::~group()
  {
    this->_data.~T();
  }

  template<class T>
  void
  group<T>::set_path(const char* path)
  {
#ifndef NDEBUG
    check_path(path);
#endif
    strcpy(this->_path, path);
  }

  template<class T>
  const char*
  group<T>::path() const
  {
    return this->_path;
  }

  template<class T>
  const T&
  group<T>::data() const
  {
    return this->_data;
  }

  template<class T>
  T&
  group<T>::data()
  {
    return this->_data;
  }

  template<class T>
  int
  group<T>::compare( const char* path ) const
  {
    return strcmp(this->_path, path);
  }

}

#endif
