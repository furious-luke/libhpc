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

#include <rapidxml_print.hpp>
#include "xml.hh"

using namespace rapidxml;

namespace hpc {
   namespace options {

      xml::xml()
         : _sep( "-" )
      {
      }

      xml::~xml()
      {
      }

      void
      xml::read( const hpc::string& filename,
                 dictionary& dict )
      {
         std::ifstream file( filename, std::fstream::in );
         read( file, dict );
      }

      void
      xml::read( std::istream& strm,
                 dictionary& dict )
      {
         // Read in the whole stream.
         std::istreambuf_iterator<char> eos;
         std::string buf( std::istreambuf_iterator<char>( strm ), eos );

         // Parse the document.
         xml_document<> doc;
         doc.parse<0>( (char*)buf.c_str() );

         // Process each root node.
         xml_node<>* node = doc.first_node();
         while( node )
         {
            _iter_node( node, "", dict );
            node = node->next_sibling();
         }
      }

      void
      xml::write( const hpc::string& filename,
                  const dictionary& dict )
      {
         std::ofstream file( filename, std::fstream::out );
         write( file, dict );
      }

      void
      xml::write( std::ostream& strm,
                  const dictionary& dict )
      {
         // Iterate over dictionary.
         xml_document<> doc;
         _iter_dict( doc, &doc, dict );

         // Dump document to stream.
         strm << doc;
      }

      void
      xml::_iter_node( xml_node<>* node,
                       hpc::string name,
                       dictionary& dict )
      {
         // Process the node.
         hpc::string sub_name;
         if( !name.empty() )
            sub_name = name + _sep;
         sub_name += node->name();
         dict[sub_name] = node->value();

         // Iterate over each child.
         xml_node<>* child = node->first_node();
         while( child )
         {
            // Don't iterate if the name is empty.
            if( strcmp( child->name(), "" ) )
               _iter_node( child, sub_name, dict );
            child = child->next_sibling();
         }
      }

      void
      xml::_iter_dict( xml_document<>& doc,
                       xml_node<>* node,
                       const dictionary& dict )
      {
         for( auto it = dict.options_begin(); it != dict.options_end(); ++it )
         {
            // Don't store if it's using the default value.
            if( !(*it)->has_value() )
               continue;

            xml_node<>* new_node = doc.allocate_node( node_element, doc.allocate_string( (*it)->name().c_str() ) );
            hpc::string val_str = (*it)->store();
            new_node->value( doc.allocate_string( val_str.c_str() ) );
            node->append_node( new_node );
         }

         for( auto it = dict.dicts_begin(); it != dict.dicts_end(); ++it )
         {
            xml_node<>* new_node = doc.allocate_node( node_element, doc.allocate_string( (*it)->prefix().c_str() ) );
            node->append_node( new_node );
            _iter_dict( doc, new_node, **it );
         }
      }
   }
}
