#ifdef HAVE_GLUT

#include "libhpc/debug/assert.hh"
#include "console.hh"
#include "text.hh"

namespace hpc {
   namespace gl {

      console::console()
         : _scr{ { 0, 0 } },
           _num_lines( 10 ),
           _line_height( 15 ),
           _alpha( 0.5 ),
           _open( 0 ),
           _anim( 0, 10*15 ),
           _cur_x( 2 ),
           _cur_on( true ),
           _cur_msecs( 0 ),
           _cur_flash( 500 )
      {
      }

      void
      console::reshape( uint16_t width,
                        uint16_t height )
      {
         _scr[0] = width;
         _scr[1] = height;
      }

      void
      console::toggle()
      {
         if( _open == 1 )
            close();
         else
            open();
      }

      void
      console::open()
      {
         _anim.reset();
         _open = 1;
         _cur_old = inter::animation<GLfloat>::clock_type::now();
      }

      void
      console::close()
      {
         _anim.reset( false );
         _open = 2;
      }

      bool
      console::display()
      {
         if( _open )
         {
            if( !_anim.done() )
               ++_anim;
            draw();
            if( _open == 2 && _anim.done() )
               _open = 0;
            return _anim.done();
         }
         else
            return true;
      }

      bool
      console::idle()
      {
         if( _open )
            return _update_cursor();
         else
            return false;
      }

      console::keyboard_state_type
      console::keyboard( int key )
      {
         if( !_open )
            return MISSED;

         if( (key >= ' ' && key <= '_') ||
             (key >= 'a' && key <= '}') )
         {
            _line += (char)key;
            _cur_x += glutBitmapWidth( GLUT_BITMAP_9_BY_15, key );
            return CAUGHT;
         }
         else if( key == 8 )
         {
            if( !_line.empty() )
            {
               _cur_x -= glutBitmapWidth( GLUT_BITMAP_9_BY_15, _line.back() );
               _line.pop_back();
               return CAUGHT;
            }
         }
         else if( key == 13 )
         {
            _hist.push_back( _line );
            _line.clear();
            _cur_x = 2;
            return ACTION;
         }

         return MISSED;
      }

      void
      console::draw()
      {
         _set_persp();
         glTranslatef( 0, *_anim - _num_lines*_line_height, 0 );
         glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LINE_BIT | GL_POLYGON_BIT );
         _draw_bg();
         _draw_hist();
         _draw_cursor();
         glPopAttrib();
         _unset_persp();
      }

      void
      console::write( const std::string& str )
      {
         _hist.push_back( str );
      }

      const std::string&
      console::line() const
      {
         ASSERT( !_hist.empty(), "No lines in empty history." );
         return _hist.back();
      }

      void
      console::_set_persp()
      {
         glMatrixMode( GL_PROJECTION );
         glPushMatrix();
         glLoadIdentity();
         gluOrtho2D( 0, _scr[0], 0, _scr[1] );
         glMatrixMode( GL_MODELVIEW );
         glPushMatrix();
         glLoadIdentity();
      }

      void
      console::_unset_persp()
      {
         glMatrixMode( GL_PROJECTION );
         glPopMatrix();
         glMatrixMode( GL_MODELVIEW );
         glPopMatrix();
      }

      void
      console::_draw_bg()
      {
         // Need to make sure the geometry works.
         ASSERT( _num_lines*_line_height < _scr[1], "Console geometry doesn't fit on screen." );

         // Now draw the background.
         glDisable( GL_LIGHTING );
         glDisable( GL_DEPTH_TEST );
         glEnable( GL_BLEND );
         glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
         glPolygonMode( GL_FRONT, GL_FILL );
         glColor4f( 0, 0, 0, _alpha );
         glBegin( GL_QUADS );
         glVertex2i( 0, 0 );
         glVertex2i( _scr[0], 0 );
         glVertex2i( _scr[0], _num_lines*_line_height );
         glVertex2i( 0, _num_lines*_line_height );
         glEnd();
      }

      void
      console::_draw_hist()
      {
         glDisable( GL_BLEND );
         glColor3f( 0.7, 0.7, 0.7 );

         draw_text_fit( _line.begin(), _line.end(), 0, 4, _scr[0] );

         unsigned ii = 1;
         for( auto it = _hist.rbegin();
              it != _hist.rend() && ii < _num_lines;
              ++it, ++ii )
         {
            draw_text_fit( it->begin(), it->end(), 0, 4 + ii*_line_height, _scr[0] );
         }
      }

      bool
      console::_update_cursor()
      {
         time_point_type cur_new = clock_type::now();
         double dur = boost::chrono::duration_cast<time_type>( cur_new - _cur_old ).count();
         if( dur > _cur_flash )
         {
            if( _cur_on )
               _cur_on = false;
            else
               _cur_on = true;
            _cur_old = cur_new;
            return true;
         }
         else
            return false;
      }

      void
      console::_draw_cursor()
      {
         if( _cur_on )
         {
            glLineWidth( 2 );
            glBegin( GL_LINES );
            glVertex2f( _cur_x, 0 );
            glVertex2f( _cur_x, _line_height );
            glEnd();
         }
      }

   }
}

#endif
