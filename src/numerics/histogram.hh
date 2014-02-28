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

#ifndef libhpc_numerics_histogram_hh
#define libhpc_numerics_histogram_hh

#include <vector>
#include <array>
#include <memory>
#include "libhpc/containers/view.hh"

namespace hpc {
   namespace numerics {

      template< class T = double >
      class histogram_axis
      {
      public:

         typedef T value_type;

      public:

         histogram_axis()
            : _rng{ 0, 0 }
         {
         }

         histogram_axis( unsigned nbins,
                         std::array<value_type,2> const& rng )
         {
            resize( nbins, rng );
         }

         void
         resize( unsigned nbins,
                 std::array<value_type,2> const& rng )
         {
            _nbins = nbins;
            _rng = rng;
            if( nbins > 0 )
               _edges.resize( nbins - 1 );
            else
               _edges.resize( 0 );
            for( unsigned ii = 0; ii < nbins - 1; ++ii )
               _edges[ii] = (double)rng[0] + ((double)(ii + 1))*((rng[1] - rng[0])/(double)nbins);
         }

         unsigned
         size() const
         {
            return _nbins;
         }

         unsigned
         bin( value_type const& x ) const
         {
            auto lb = std::lower_bound( _edges.begin(), _edges.end(), x );
            return lb - _edges.begin();
         }

      protected:

         unsigned _nbins;
         std::array<value_type,2> _rng;
         std::vector<value_type> _edges;
      };

      template< class T = double >
      class histogram
      {
      public:

         typedef T value_type;
         typedef histogram_axis<value_type> axis_type;

      public:

         histogram()
         {
         }

         histogram( unsigned nbins,
                    std::array<value_type,2> const& rng )
         {
            resize( nbins, rng );
         }

         histogram( std::shared_ptr<axis_type>& axis )
            : _axis( axis ),
              _bins( axis->size() )
         {
         }

         std::shared_ptr<axis_type>&
         axis()
         {
            return _axis;
         }

         void
         resize( unsigned nbins,
                 std::array<value_type,2> const& rng )
         {
            _axis.reset( new axis_type( nbins, rng ) );
            _bins.resize( nbins );
            reset();
         }

         void
         reset()
         {
            std::fill( _bins.begin(), _bins.end(), 0.0 );
         }

         void
         add_sample( value_type x )
         {
            ++_bins[_axis->bin( x )];
         }

         hpc::view<std::vector<unsigned>>::type const
         bins() const
         {
            return _bins;
         }

      protected:

         std::shared_ptr<axis_type> _axis;
         std::vector<unsigned> _bins;
      };

   }
}

#endif
