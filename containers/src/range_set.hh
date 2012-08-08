#ifndef hpc_containers_range_set_hh
#define hpc_containers_range_set_hh

#include "set.hh"
#include "range.hh"
#include "list.hh"

namespace hpc {

   template< class T >
   class range_set
      : public set< range<T> >
   {
   public:

      typedef set<range<T>> super_type;
      typedef range<T> range_type;
      typedef typename set<std::pair<T, T>>::iterator iterator;

      void
      insert( const range_type& range )
      {
         auto low = this->lower_bound( range );

         // If the range is missing, add it in.
         if( low == this->end() )
         {
            super_type::insert( range );
         }

         // If there was some overlap we will need to split it.
         else if( *low != range )
         {
            auto cur = low;
            auto upp = this->upper_bound( range );
            list<range_type> new_ranges;
            vector<range_type> split_ranges;
            while( cur != upp )
            {
               cur->split( range, split_ranges );

               // First iteration takes first and second.
               if( cur == low )
               {
                  new_ranges.push_back( split_ranges[0] );
                  if( split_ranges.size() > 0 )
                     new_ranges.push_back( split_ranges[1] );
               }

               // Middle iterations take second.
               else
                  new_ranges.push_back( split_ranges[1] );

               ++cur;
            }

            // Push last section on.
            new_ranges.push_back( split_ranges.back() );

            // Erase existing ranges.
            this->erase( low, upp );

            // Insert my new ones.
            this->insert( new_ranges.begin(), new_ranges.end() );
         }
      }

      template< class Iterator >
      void
      insert( Iterator start,
              const Iterator& finish )
      {
         super_type::insert( start, finish );
      }
   };
}

#endif
