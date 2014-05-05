#ifndef libhpc_interactive_colour_map_hh
#define libhpc_interactive_colour_map_hh

#include <vector>
#include <boost/array.hpp>

namespace hpc {
   namespace gl {

      template< class S,
                class C = float,
                int N = 3 >
      class colour_map
      {
      public:

         enum { num_fields_const = N };

         typedef S source_type;
         typedef C colour_field_type;
         typedef boost::array<colour_field_type,num_fields_const> colour_type;

      public:

         void
         set_abscissa( std::vector<source_type>& abs )
         {
            _abs.deallocate();
            _abs.swap( abs );
            _w = abs.back() - abs.front();
            ASSERT( _w > 0.0, "Abscissa must span a length greater than zero." );
            _w = 1.0/_w;

#ifndef NDEBUG
            // Must be in ascending or descending order.
            bool asc = abs.back() > abs.front();
            for( unsigned ii = 1; ii < abs.size(); ++ii )
            {
               if( asc )
                  ASSERT( abs[ii] > abs[ii - 1], "Must be all either ascending or descending ordered." );
               else
                  ASSERT( abs[ii] < abs[ii - 1], "Must be all either ascending or descending ordered." );
            }
#endif
         }

         void
         set_colours( std::vector<colour_type>& cols )
         {
            _cols.deallocate();
            _cols.swap( cols );
         }

         void
         set_colours_diverging_11()
         {
            _cols.reallocate( 11 );
            _cols[0][0]  = 94.0/255.0;  _cols[0][1]  = 79.0/255.0;  _cols[0][2] = 162.0/255.0;
            _cols[1][0]  = 50.0/255.0;  _cols[1][1]  = 136.0/255.0; _cols[1][2] = 189.0/255.0;
            _cols[2][0]  = 102.0/255.0; _cols[2][1]  = 194.0/255.0; _cols[2][2] = 165.0/255.0;
            _cols[3][0]  = 171.0/255.0; _cols[3][1]  = 221.0/255.0; _cols[3][2] = 164.0/255.0;
            _cols[4][0]  = 230.0/255.0; _cols[4][1]  = 245.0/255.0; _cols[4][2] = 152.0/255.0;
            _cols[5][0]  = 255.0/255.0; _cols[5][1]  = 255.0/255.0; _cols[5][2] = 191.0/255.0;
            _cols[6][0]  = 254.0/255.0; _cols[6][1]  = 224.0/255.0; _cols[6][2] = 139.0/255.0;
            _cols[7][0]  = 253.0/255.0; _cols[7][1]  = 174.0/255.0; _cols[7][2] = 97.0/255.0;
            _cols[8][0]  = 244.0/255.0; _cols[8][1]  = 109.0/255.0; _cols[8][2] = 67.0/255.0;
            _cols[9][0]  = 213.0/255.0; _cols[9][1]  = 62.0/255.0;  _cols[9][2] = 79.0/255.0;
            _cols[10][0] = 158.0/255.0; _cols[10][1] = 1.0/255.0;   _cols[10][2] = 66.0/255.0;
         }

         void
         set_colours_diverging_blue_to_red_12()
         {
            _cols.reallocate( 12 );
            _cols[0][0]  = 41.0/255.0;  _cols[0][1]  = 10.0/255.0;  _cols[0][2] = 216.0/255.0;
            _cols[1][0]  = 28.0/255.0;  _cols[1][1]  = 77.0/255.0;  _cols[1][2] = 255.0/255.0;
            _cols[2][0]  = 63.0/255.0;  _cols[2][1]  = 160.0/255.0; _cols[2][2] = 255.0/255.0;
            _cols[3][0]  = 114.0/255.0; _cols[3][1]  = 217.0/255.0; _cols[3][2] = 255.0/255.0;
            _cols[4][0]  = 170.0/255.0; _cols[4][1]  = 247.0/255.0; _cols[4][2] = 255.0/255.0;
            _cols[5][0]  = 224.0/255.0; _cols[5][1]  = 255.0/255.0; _cols[5][2] = 255.0/255.0;
            _cols[6][0]  = 255.0/255.0; _cols[6][1]  = 255.0/255.0; _cols[6][2] = 191.0/255.0;
            _cols[7][0]  = 255.0/255.0; _cols[7][1]  = 224.0/255.0; _cols[7][2] = 153.0/255.0;
            _cols[8][0]  = 255.0/255.0; _cols[8][1]  = 173.0/255.0; _cols[8][2] = 114.0/255.0;
            _cols[9][0]  = 247.0/255.0; _cols[9][1]  = 109.0/255.0; _cols[9][2] = 94.0/255.0;
            _cols[10][0] = 216.0/255.0; _cols[10][1] = 38.0/255.0;  _cols[10][2] = 50.0/255.0;
            _cols[11][0] = 165.0/255.0; _cols[11][1] = 0.0/255.0;   _cols[11][2] = 33.0/255.0;
         }

         void
         set_abscissa_linear( source_type min,
                              source_type max )
         {
            _abs.reallocate( _cols.size() );
            if( _abs.size() )
            {
               _abs.front() = min;
               if( _abs.size() > 1 )
               {
                  _abs.back() = max;
                  for( unsigned ii = 1; ii < _abs.size() - 1; ++ii )
                  {
                     source_type fac = (source_type)ii/(source_type)(_abs.size() - 1);
                     _abs[ii] = min + fac*(max - min);
                  }
               }
            }
         }

         unsigned
         num_points() const
         {
            return _cols.size();
         }

         const colour_type&
         colour( unsigned idx ) const
         {
            return _cols[idx];
         }

         const source_type&
         abscissa( unsigned idx ) const
         {
            return _abs[idx];
         }

         void
         map( const source_type& src,
              colour_type& col )
         {
            auto idx = _index( src );
            const colour_type& first = _cols[idx.first];
            const colour_type& last = _cols[idx.first + 1];
            for( unsigned ii = 0; ii < first.size(); ++ii )
               col[ii] = first[ii] + idx.second*(last[ii] - first[ii]);
         }

         colour_type
         operator[]( const source_type& src )
         {
            colour_type col;
            map( src, col );
            return col;
         }

      protected:

         std::pair<unsigned,source_type>
         _index( const source_type& src )
         {
            auto it = std::lower_bound( _abs.begin(), _abs.end(), src );
            if( it == _abs.end() )
               return std::make_pair<unsigned,source_type>( _cols.size() - 2, 1.0 );
            else if( it == _abs.begin() )
               return std::make_pair<unsigned,source_type>( 0, 0.0 );
            else
            {
               auto last = it--;
               return std::make_pair<unsigned,source_type>( it - _abs.begin(), (src - *it)/(*last - *it) );
            }
         }

      protected:

         std::vector<colour_type> _cols;
         std::vector<source_type> _abs;
         source_type _w;
      };

   }
}

#endif
