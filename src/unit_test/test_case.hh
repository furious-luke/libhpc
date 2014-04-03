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

#ifndef libhpc_unit_test_test_case_hh
#define libhpc_unit_test_test_case_hh

#define TEST_CASE( name )                               \
   void UNIQUE_LINE( __hpc_test_case__ )();             \
   ::hpc::test::test_case<> ANON(                       \
      name, "", UNIQUE_LINE( __hpc_test_case__ ) );     \
   void UNIQUE_LINE( __hpc_test_case__ )()

namespace hpc {
   namespace test {

      class test_case_base
      {
      public:

         test_case_base( const std::string& name,
                         const std::string& desc );

         virtual
         void
         run() = 0;

         std::string const&
         name() const;

         std::string const&
         description() const;

      protected:

         void
         _add_test_case();

      protected:

         std::string _name;
         std::string _desc;
      };

      template< class Fixture >
      class test_case
         : public test_case_base
      {
      public:

         test_case( const std::string& name,
                    const std::string& desc,
                    boost::function<void(Fixture&)> func )
            : test_case_base( name, desc ),
              _func( func ),
              _fix( 0 )
         {
         }

         virtual
         ~test_case()
         {
            if( _fix )
            {
               delete _fix;
               _fix = 0;
            }
         }

         virtual
         void
         run()
         {
            _cur_tc = this;
            _fix = new Fixture;
            _func( *_fix );
            delete _fix;
            _fix = 0;
         }

      protected:

         std::function<void(Fixture&)> _func;
         Fixture* _fix;
      };

      template<>
      class test_case<void>
         : public test_case_base
      {
      public:

         test_case( const std::string& name,
                    const std::string& desc,
                    std::function<void()> func )
            : test_case_base( name, desc ),
              _func( func )
         {
         }

         virtual
         void
         run()
         {
            _cur_tc = this;
            _func();
         }

      protected:

         std::function<void()> _func;
      };

   }
}

#endif
