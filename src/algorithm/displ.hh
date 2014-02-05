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

#ifndef hpc_algorithm_displ_hh
#define hpc_algorithm_displ_hh

namespace hpc {
   namespace algorithm {

      template< class InputIter,
                class OutputIter >
      unsigned
      displ( InputIter start,
             InputIter const& finish,
             OutputIter result,
             typename OutputIter::value_type offs = 0 )
      {
         *result++ = offs;
         while( start != finish )
         {
            typename OutputIter::value_type tmp = offs + *start++;
            *result++ = tmp;
            offs = tmp;
         }
      }

      template< class InputIter >
      unsigned
      displ_in_place( InputIter start,
                      InputIter const& finish,
                      typename InputIter::value_type offs = 0 )
      {
         if( start != finish )
         {
            typename InputIter::value_type prev = *start + offs;
            *start++ = offs;
            while( start != finish )
            {
               typename InputIter::value_type tmp = prev + *start;
               *start++ = prev;
               prev = tmp;
            }
         }
      }

   }
}

#endif
