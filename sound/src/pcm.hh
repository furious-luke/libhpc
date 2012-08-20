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

#ifndef libhpc_sound_pcm_hh
#define libhpc_sound_pcm_hh

#include <alsa/asoundlib.h>
#include "libhpc/system/pipe.hh"

namespace hpc {
   namespace sound {

      class pcm
      {
      public:

         pcm();

         ~pcm();

         void
         open();

         void
         close();

         unsigned
         sample_rate() const;

         unsigned long
         period_size() const;

         long
         available_frames();

         const vector<unix::pipe>&
         poll_pipes() const;

      protected:

         void
         _set_hw_params();

         void
         _set_sw_params();

         void
         _get_poll_fds();

      protected:

         snd_pcm_t* _handle;
         unsigned _rate;
         snd_pcm_uframes_t _frames;
         vector<unix::pipe> _fds;
      };
   }
}

#endif
