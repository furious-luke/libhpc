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

#ifndef libhpc_options_xml_dict_hh
#define libhpc_options_xml_dict_hh

#include <iostream>
#include <typeinfo>
#include <sstream>
#include <pugixml.hpp>
#include "libhpc/containers/list.hh"
#include "libhpc/containers/string.hh"

namespace hpc {
   namespace options {
      using namespace pugi;

      class xml_dict
      {
      public:

      public:

         xml_dict();

         ~xml_dict();

         void
         read( const string& filename,
               const string& path=string() );

         void
         read( std::istream& stream,
               const string& path=string() );

         template< class T >
         T
         get( const string& path ) const
         {
            return _coerce<T>( _get_node( path ).first_child().value() );
         }

         template< class T >
         list<T>
         get_list( const string& path ) const
         {
            xml_node node = _get_node( path );
            list<T> val;
            for( xml_node_iterator it = node.begin(); it != node.end(); ++it )
            {
               if( it->first_child() && it->first_child().type() == node_pcdata )
                  val.push_back( _coerce<T>( it->first_child().value() ) );
            }
            return val;
         }

      protected:

         xml_node
         _find_root( xml_node& node,
                     const string& path ) const;

         void
         _merge( std::istream& stream,
                 const string& path );

         void
         _merge_node( xml_node merge_into,
                      xml_node merge_from );

         xml_node
         _get_node( const string& path ) const;

         string
         _xform_path( const string& path ) const;

         template< class T >
         T
         _coerce( const string& value ) const
         {
            std::stringstream ss( value );
            T val;
            ss >> val;
            if( ss.fail() )
               throw std::bad_cast();
            return val;
         }

      protected:

         string _sep;
         xml_document _doc;
         xml_node _root;
      };
   }
}

#endif
