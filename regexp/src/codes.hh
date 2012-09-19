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

#ifndef libhpc_regexp_codes_hh
#define libhpc_regexp_codes_hh

namespace hpc {
   namespace re {

      enum codes
      {
         code_match = 0,
         code_concat = 127,
         code_split = 128,
         code_many = 129,
         code_capture = 130,
         code_class_all = 131,
         code_class_digit = 132,
         code_terminal = 133
      };

      extern byte classes[];
   }
}

#endif
