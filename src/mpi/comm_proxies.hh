#ifndef hpc_mpi_comm_proxies_hh
#define hpc_mpi_comm_proxies_hh

namespace hpc {
   namespace mpi {

      template< class T,
		class Enable = void >
      struct scan_proxy
      {
      };

      template< class T >
      struct scan_proxy<T,typename boost::disable_if<hpc::is_fundamental_r<T> >::type>
      {
	 typedef std::vector<typename type_traits<T>::value::value_type> result_type;
      };

      template< class T >
      struct scan_proxy<T,typename boost::enable_if<hpc::is_fundamental_r<T> >::type>
      {
	 typedef typename type_traits<T>::value result_type;
      };

   }
}

#endif
