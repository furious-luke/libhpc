#ifndef libhpc_interactive_command_hh
#define libhpc_interactive_command_hh

#include <boost/regex.hpp>
#include <boost/function.hpp>

namespace hpc {
   namespace command {

      class command
      {
      public:

         typedef boost::function<void(boost::smatch const&)> function_type;

      public:

         command();

         command( std::string const& expr,
                  function_type action );

         std::string const&
         expression() const;

         boost::regex const&
         re() const;

         void
         operator()( boost::smatch const& match ) const;

      protected:

         std::string _re_str;
         boost::regex _re;
         function_type _act;
      };

   }
}

#endif
