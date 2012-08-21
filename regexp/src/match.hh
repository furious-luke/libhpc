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

#ifndef libhpc_regexp_match_hh
#define libhpc_regexp_match_hh

#include "libhpc/containers/vector.hh"

namespace hpc {
   namespace re {

      class match
      {
         friend class dfa;

      public:

         typedef std::pair<const char*,const char*> capture_type;

      public:

         uint16
         last_capture() const;

         uint16
         num_captures() const;

         const capture_type&
         capture( uint16 idx ) const;

      protected:

         uint16 _last;
         vector<std::pair<const char*,const char*>> _caps;
      };
   }
}

#endif
