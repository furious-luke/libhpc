#include "shuffle_mapping.hh"

namespace hpc {

   shuffle_mapping::shuffle_mapping()
   {
   }

   shuffle_mapping::~shuffle_mapping()
   {
   }

   void
   shuffle_mapping::setup( vector<index>& mobile_indices,
			   shuffle_type _type )
   {
      this->_mobile_idxs.clear();
      this->_mobile_idxs.swap(mobile_indices);
      this->_type = _type;
   }
}
