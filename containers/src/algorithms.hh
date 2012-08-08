#ifndef hpc_containers_algorithms_hh
#define hpc_containers_algorithms_hh

namespace hpc {

   template< class InputIter,
	     class OutputMap >
   void
   sequence_to_map( InputIter begin,
		    const InputIter& end,
		    OutputMap& out)
   {
      int ii = 0;
      while(begin != end) {
	 out.insert(std::make_pair(ii, *begin));
	 ++begin;
	 ++ii;
      }
   }

   template< class InputIter,
	     class OutputMap >
   void
   sequence_to_reverse_map( InputIter begin,
			    const InputIter& end,
			    OutputMap& out)
   {
      int ii = 0;
      while(begin != end) {
	 out.insert(std::make_pair(*begin, ii));
	 ++begin;
	 ++ii;
      }
   }
}

#endif
