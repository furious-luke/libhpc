#ifndef debug_group_hh
#define debug_group_hh

namespace debug {

   template< class T >
   class group_context;

   template<class T>
   class group {
   public:

      static const int max_path_length = 100;

      group();

      ~group();

      void
      set_path( const char* path );

      const char*
      path() const;

      const T&
      data() const;

      T&
      data();

   protected:

      int compare(const char* path) const;

   private:

      T _data;
      char _path[max_path_length + 1];
      int _left;
      int _right;
      int _select_idx;
      int _select_cnt;

      friend class group_context<T>;
   };
}

#endif
