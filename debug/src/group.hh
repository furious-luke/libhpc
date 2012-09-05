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

#ifndef debug_group_hh
#define debug_group_hh

namespace hpc {
   namespace debug {

      template< class T >
      class group_context;

      template<class T>
      class group {
      public:

         static const int max_path_length = 100;

         group();

         ~group();

         void
         set_path( const char* path );

         const char*
         path() const;

         const T&
         data() const;

         T&
         data();

      protected:

         int compare(const char* path) const;

      private:

         T _data;
         char _path[max_path_length + 1];
         int _left;
         int _right;
         int _select_idx;
         int _select_cnt;

         friend class group_context<T>;
      };
   }
}

#endif
