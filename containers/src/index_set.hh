#ifndef libhpc_containers_index_set_hh
#define libhpc_containers_index_set_hh

#include "vector.hh"

namespace hpc {

   class index_set
   {
   public:

      static const byte_t idx_add[8];
      static const byte_t idx_rem[8];

      index_set( index max_size=0 );

      ~index_set();

      void
      clear();

      void
      set_max_size( index max_size );

      void
      merge( index idx );

      void
      remove( index idx );

      void
      set_index( index idx,
		 bool state );

      bool
      has( index idx ) const;

      const vector<byte_t>&
      bits() const;

      int
      bit( index idx ) const;

   private:
      index _max_size;
      index _size;
      vector<byte_t> _bits;
   };
}

#endif
