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

#ifndef hpc_numerics_constants_hh
#define hpc_numerics_constants_hh

namespace hpc {
   namespace constant {

      // Astronomy/astrophysics.
      static const double c           = 2.99792458e8; // m/s
      static const double c_km_s      = 299792.458; // m/s
      static const double c_mpc_gyr   = 3.06391536e2; // Mpc/Gyr
      static const double parsec      = 3.08567758e16; // m
      static const double mpc         = 3.08567758e22; // m
      static const double gravitation = 6.672e-8; // ?

      // Mathematical.
      static const double pi = M_PI;

   }
}

#endif
