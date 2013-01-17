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

#ifndef libhpc_options_xml_hh
#define libhpc_options_xml_hh

#include <iostream>
#include <pugixml.hpp>
#include "libhpc/containers/string.hh"
#include "format.hh"
#include "dictionary.hh"

namespace hpc {
   namespace options {

      class xml
         : public format
      {
      public:

         xml();

         ~xml();

         void
         read( const hpc::string& filename,
               dictionary& dict,
               const hpc::string& path=hpc::string() );

         void
         read( std::istream& strm,
               dictionary& dict,
               const hpc::string& path=hpc::string() );

         void
         write( const hpc::string& filename,
                const dictionary& dict );

         void
         write( std::ostream& filename,
                const dictionary& dict );

         virtual
         void
         start_list( const hpc::string& name );

         virtual
         void
         add_list_item( const hpc::string& value );

         virtual
         void
         end_list();

      protected:

         void
         _iter_node( pugi::xml_node& node,
                     hpc::string name,
                     dictionary& dict );

         void
         _iter_dict( pugi::xml_node& node,
                     const dictionary& dict );

         hpc::string
         _node_name( pugi::xml_node& node ) const;

      protected:

         hpc::string _sep;
         pugi::xml_node* _cur_node;
      };
   }
}

#endif
