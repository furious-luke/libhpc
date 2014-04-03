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

#ifndef libhpc_system_anon_hh
#define libhpc_system_anon_hh

#define ANON2( x, y )                           \
   x##y
#define ANON1( x, y )                           \
   ANON2( x, y )
#define ANON                                    \
   ANON1( __hpc__, __COUNTER__ )

#define UNIQUE_LINE2( x, y )                    \
   x##y
#define UNIQUE_LINE1( x, y )                    \
   UNIQUE_LINE2( x, y )
#define UNIQUE_LINE( x )                        \
   UNIQUE_LINE1( x, __LINE__ )

#endif
