#ifndef libhpc_interactive_console_hh
#define libhpc_interactive_console_hh

#ifdef HAVE_GLUT

#include <GL/gl.h>
#include <GL/glu.h>
#include "libhpc/system/types.hh"
#include "libhpc/containers/containers.hh"
#include "animation.hh"

namespace hpc {
   namespace gl {

      class console
      {
      public:

         enum keyboard_state_type
         {
            ACTION,
            CAUGHT,
            MISSED
         };

      public:

         console();

         void
         reshape( uint16 width,
                  uint16 height );

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
         write( const string& str );

         const string&
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

         array<uint16,2> _scr;
         uint16 _num_lines;
         uint16 _line_height;
         GLfloat _alpha;
         int _open;
         list<string> _hist;
         string _line;
         inter::animation<GLfloat> _anim;

         GLfloat _cur_x;
         bool _cur_on;
         unsigned _cur_msecs;
         unsigned _cur_flash;
         posix::time_type _cur_old;
      };

   }
}

#endif

#endif
