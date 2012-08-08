#ifndef hpc_containers_range_map_hh
#define hpc_containers_range_map_hh

#include "map.hh"
#include "set.hh"
#include "range.hh"
#include "list.hh"

namespace hpc {

   ///
   ///
   ///
   template< class T,
             class Value >
   class range_map
      : public map< range<T>, set<Value> >
   {
   public:

      typedef map<range<T>, set<Value>> super_type;
      typedef range<T> range_type;
      typedef set<Value> mapped_type;
      typedef typename set<std::pair<T, T>>::iterator iterator;

      void
      insert( const range_type& range,
              const Value& value )
      {
         auto low = this->lower_bound( range );

         // If the range is missing, add it in. Arrgh! Need to check if
         // the range found is entirely above the requested one!
         if( low == this->end() || low->first.start() >= range.finish() )
         {
            set<Value> tmp;
            tmp.insert( value );
            super_type::insert( range, tmp );
         }

         // If there was some overlap we will need to split it.
         else if( low->first != range )
         {
            auto cur = low;
            auto upp = this->upper_bound( range );
            list<std::pair<range_type, mapped_type>> new_ranges;
            vector<range_type> split_ranges;
            while( cur != upp )
            {
               cur->first.split( range, split_ranges );

               // First iteration takes first and second.
               if( cur == low )
               {
                  // Only take the existing mapped value if the existing range begins
                  // before the new range.
                  if( cur->first.start() <= range.start() )
                     new_ranges.push_back( std::make_pair( split_ranges[0], cur->second ) );
                  else
                     new_ranges.push_back( std::make_pair( split_ranges[0], mapped_type() ) );
                  if( split_ranges.size() > 0 && cur->first.start() != range.start() )
                     new_ranges.push_back( std::make_pair( split_ranges[1], cur->second ) );
               }

               // Middle iterations take second.
               else
                  new_ranges.push_back( std::make_pair( split_ranges[1], cur->second ) );

               ++cur;
            }

            // Sanity check.
            ASSERT( split_ranges.size() );

            // Push last section on if it extends beyond.
            --cur;
            if( cur->first.finish() != range.finish() )
            {
               if( cur->first.finish() > range.finish() )
               {
                  new_ranges.push_back( std::make_pair( split_ranges.back(), cur->second ) );
               }
               else
                  new_ranges.push_back( std::make_pair( split_ranges.back(), mapped_type() ) );
            }

            // Add the new value to each set.
            for( auto& elem : new_ranges )
            {
               if( elem.first.start() < range.finish() && elem.first.finish() > range.start() )
                  elem.second.insert( value );
            }

            // Erase existing ranges.
            this->erase( low, upp );

            // Insert my new ones.
            for( auto elem : new_ranges )
               super_type::insert( elem.first, elem.second );
         }

         // They're equal, so just add the new value.
         else
         {
            low->second.insert( value );
         }
      }

      void
      insert( const range_type& range )
      {
         auto low = this->lower_bound( range );

         // If the range is missing, add it in.
         if( low == this->end() )
         {
            set<Value> tmp;
            super_type::insert( range, tmp );
         }

         // If there was some overlap we will need to split it.
         else if( low->first != range )
         {
            auto cur = low;
            auto upp = this->upper_bound( range );
            list<std::pair<range_type, mapped_type>> new_ranges;
            vector<range_type> split_ranges;
            while( cur != upp )
            {
               cur->first.split( range, split_ranges );

               // First iteration takes first and second.
               if( cur == low )
               {
                  // Only take the existing mapped value if the existing range begins
                  // before the new range.
                  if( cur->first.start() <= range.start() )
                     new_ranges.push_back( std::make_pair( split_ranges[0], cur->second ) );
                  else
                     new_ranges.push_back( std::make_pair( split_ranges[0], mapped_type() ) );
                  if( split_ranges.size() > 0 && cur->first.start() != range.start() )
                     new_ranges.push_back( std::make_pair( split_ranges[1], cur->second ) );
               }

               // Middle iterations take second.
               else
                  new_ranges.push_back( std::make_pair( split_ranges[1], cur->second ) );

               ++cur;
            }

            // Push last section on if it extends beyond.
            --cur;
            if( cur->first.finish() != range.finish() )
            {
               if( cur->first.finish() > range.finish() )
               {
                  new_ranges.push_back( std::make_pair( split_ranges.back(), cur->second ) );
               }
               else
                  new_ranges.push_back( std::make_pair( split_ranges.back(), mapped_type() ) );
            }

            // Erase existing ranges.
            this->erase( low, upp );

            // Insert my new ones.
            for( auto elem : new_ranges )
               super_type::insert( elem.first, elem.second );
         }
      }
   };

   ///
   ///
   ///
   template< class T,
             class Value >
   void
   invert_mapping( const range_map<T, Value>& mapping,
                   map<Value, set<range<T>>>& inv )
   {
      for( const auto& elem : mapping )
      {
         for( const auto& val : elem.second )
         {
            // TODO: Combine sequential ranges.
            inv[val].insert( elem.first );
         }
      }
   }
}

#endif
