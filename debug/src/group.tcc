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
