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
#include "libhpc/containers/optional.hh"
#include "bad_option.hh"

namespace hpc {
   namespace options {
      using namespace pugi;

      ///
      /// Exception thrown when there is any trouble parsing XML.
      ///
      class bad_xml
         : public bad_option
      {
      public:

         bad_xml( const hpc::string& filename = hpc::string() );

         virtual
         ~bad_xml() throw();
      };

      ///
      /// A direct XML based options dictionary. The standard dictionary
      /// uses option predeclaration to allow program help and unified
      /// defaults. However, sometimes it is more convenient to not have
      /// to require that predeclaration. xml_dict provides this.
      ///
      class xml_dict
      {
      public:

         ///
         /// Default constructor.
         ///
         xml_dict();

         ///
         /// Destructor.
         ///
         ~xml_dict();

         ///
         /// Read an XML from file. If called multiple times, each subsequent
         /// XML will be merged with the existing dictionary.
         ///
         /// @param[in] filename The filename of the XML file to load.
         /// @param[in] xpath_root Optional XPath root to use.
         /// @throws bad_xml If any error occurs during XML parsing.
         ///
         void
         read( const hpc::string& filename,
               const hpc::string& xpath_root = hpc::string() );

         ///
         /// Read an XML from a C++ stream. If called multiple times, each
         /// subsequent XML will be merged with the existing dictionary.
         ///
         /// @param[in] stream The C++ stream to read from.
         /// @param[in] xpath_root Optional XPath root to use.
         /// @param[in] filename Optional filename, used for error reporting.
         /// @throws bad_xml If any error occurs during XML parsing.
         ///
         void
         read( std::istream& stream,
               const hpc::string& xpath_root = hpc::string(),
               const hpc::string& filename = hpc::string() );

         ///
         /// Test for existence of an option.
         ///
         /// @param[in] path The option path to test for.
         /// @returns Boolean describing the existence of the option.
         ///
         bool
         has( const hpc::string& path ) const;

         ///
         /// Extract and return an option value.
         ///
         /// @tparam T The type of the option.
         /// @param[in] path The option path to get the value of.
         /// @returns The value of the option.
         /// @throws bad_option Thrown when the option path does not exist.
         ///
         template< class T >
         T
         get( const hpc::string& path ) const
         {
            return _coerce<T>( _get_node( path ).first_child().value() );
         }

         ///
         /// Extract and return an option value in an optional
         /// structure.
         ///
         /// @tparam T The type of the option.
         /// @param[in] path The option path to get the value of.
         /// @returns The optional value of the option, depending on whether
         ///          the option name exists.
         ///
         template< class T >
         optional<T>
         opt( const hpc::string& path ) const
         {
            auto node = _get_node( path, false );
            if( node )
               return _coerce<T>( node.first_child().value() );
            else
               return none;
         }

         ///
         /// Extract and return an option value. If the option does not
         /// exist default_value is returned.
         ///
         /// @tparam T The type of the option.
         /// @param[in] path The option path to get the value of.
         /// @param[in] default_value The value to return if the option path
         ///                          cannot be found.
         /// @returns The value of the option.
         ///
         template< class T >
         T
         get( const hpc::string& path,
              const T& default_value ) const
         {
            auto node = _get_node( path, false );
            if( node )
               return _coerce<T>( node.first_child().value() );
            else
               return default_value;
         }

         ///
         /// Extract and return an option value as a list. An option list
         /// in XML is interpreted as being a set of subelements. Each sub-
         /// element with a pcdata child has that pcdata value appended to
         /// the list.
         ///
         /// @tparam T The type of the elements of the list.
         /// @param[in] path The option path to get the value of.
         /// @returns The list of values.
         ///
         template< class T >
         hpc::list<T>
         get_list( const hpc::string& path ) const
         {
            xml_node node = _get_node( path );
            hpc::list<T> val;
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
                     const hpc::string& xpath_root ) const;

         void
         _merge( std::istream& stream,
                 const hpc::string& path,
                 const hpc::string& filename = hpc::string() );

         void
         _merge_node( xml_node merge_into,
                      xml_node merge_from );

         xml_node
         _get_node( const hpc::string& path,
                    bool except = true ) const;

         hpc::string
         _xform_path( const hpc::string& path ) const;

         template< class T >
         T
         _coerce( const hpc::string& value ) const
         {
            std::stringstream ss( value );
            T val;
            ss >> val;
            if( ss.fail() )
               throw std::bad_cast();
            return val;
         }

      protected:

         hpc::string _sep;
         xml_document _doc;
         xml_node _root;
      };
   }
}

#endif
