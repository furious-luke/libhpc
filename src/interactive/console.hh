#ifndef hpc_interactive_console_hh
#define hpc_interactive_console_hh

#ifdef HAVE_GLUT

#include <stdint.h>
#include <string>
#include <list>
#include <array>
#include <GL/gl.h>
#include <GL/glu.h>
#include "animation.hh"

namespace hpc {
   namespace gl {

      class console
      {
      public:

         typedef          boost::chrono::high_resolution_clock clock_type;
         typedef typename clock_type::time_point               time_point_type;
         typedef          boost::chrono::milliseconds          time_type;

         enum keyboard_state_type
         {
            ACTION,
            CAUGHT,
            MISSED
         };

      public:

         console();

         void
         reshape( uint16_t width,
                  uint16_t height );

         void
         toggle();

         void
         open();

         void
         close();

         bool
         display();

         bool
         idle();

         keyboard_state_type
         keyboard( int key );

         void
         draw();

         void
         write( const std::string& str );

         const std::string&
         line() const;

      protected:

         void
         _set_persp();

         void
         _unset_persp();

         void
         _draw_bg();

         void
         _draw_hist();

         bool
         _update_cursor();

         void
         _draw_cursor();

      protected:

	 std::array<uint16_t,2> _scr;
         uint16_t _num_lines;
         uint16_t _line_height;
         GLfloat _alpha;
         int _open;
	 std::list<std::string> _hist;
	 std::string _line;
         inter::animation<GLfloat> _anim;

         GLfloat _cur_x;
         bool _cur_on;
         unsigned _cur_msecs;
         unsigned _cur_flash;
         time_point_type _cur_old;
      };

   }
}

#endif

#endif
