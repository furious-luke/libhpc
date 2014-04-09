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

#ifndef libhpc_algorithm_multimatch_hh
#define libhpc_algorithm_multimatch_hh

#include <list>
#include <string>
#include <boost/optional.hpp>
#include <boost/regex.hpp>

namespace hpc {

   class multimatch
   {
   public:

      multimatch();

      void
      clear();

      void
      add_match( std::string const& str );

      ///
      /// Note: Expecting matches to be in descending order of
      ///       length.
      ///
      template< class Iterator >
      void
      add_matches( Iterator first,
                   Iterator last )
      {
         while( first != last )
            _matches.push_back( *first++ );
      }

      void
      compile();

      boost::optional<size_t>
      match( std::string const& str,
             boost::smatch& match ) const;

      boost::optional<size_t>
      match( std::string const& str ) const;

      boost::optional<size_t>
      search( std::string const& str,
              boost::smatch& match ) const;

      boost::optional<size_t>
      search( std::string const& str ) const;

   protected:

      size_t
      _last_capture( boost::smatch& match ) const;

   protected:

      std::list<std::string> _matches;
      boost::regex _re;
      bool _ready;
   };
};

#endif
