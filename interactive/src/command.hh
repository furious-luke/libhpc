#ifndef tao_zen_command_hh
#define tao_zen_command_hh

#include <libhpc/regexp/re.hh>

namespace tao {
   using namespace hpc;

   class command
   {
   public:

      typedef std::function<void(const re::match&)> function_type;

   public:

      command();

      command( const string& re_str,
               function_type action );

      const string&
      re_string() const;

      const re::re&
      re() const;

      void
      operator()( const re::match& match ) const;

   protected:

      string _re_str;
      hpc::re::re _re;
      function_type _act;
   };

}

#endif
