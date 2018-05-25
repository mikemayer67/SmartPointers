#ifndef _SMARTPOINTERS_H_
#define _SMARTPOINTERS_H_

////////////////////////////////////////////////////////////////////////////////
//  See the README.txt file for details about the smart pointer 
//    templates defined in this header file.  There is no source file.
////////////////////////////////////////////////////////////////////////////////

#ifdef SMARTPOINTER_NAMESPACE
#if SMARTPOINTER_NAMESPACE==1
#define NS SmartPointer
#else
#define NS SMARTPOINTER_NAMESPACE
#endif
#endif

#ifdef NS
namespace NS {
#endif

  template <typename T>
    class smrt
    {
      typedef smrt<T> Type_t;

      // Constructors and Assignment

      protected: smrt(const T *p=NULL) : _ptr(p)      {}
      protected: smrt(const Type_t &p) : _ptr(p.ptr_) {}

      protected: Type_t &operator=(const T*  p)     { _ptr = p;      return *this; }
      protected: Type_t &operator=(const Type_t &p) { _ptr = p._ptr; return *this; }

      // Methods

      public: const T &operator*(void)  const { assert_set(); return *_ptr; }
      public: const T *operator->(void) const { assert_set(); return  _ptr; }
      public: const T *raw(void)        const {               return  _ptr; }

      public: bool isSet(void)     const { return _ptr!=NULL; }
      public: bool isNull(void)    const { return _ptr==NULL; }
      public: bool isNotNull(void) const { return _ptr!=NULL; }

      public: bool operator == (const Type_t &p) const { return _ptr == p._ptr; }
      public: bool operator <  (const Type_t &p) const { return _ptr <  p._ptr; }

      protected: void assert_set(void) const
                 {
                   if(_ptr==NULL) 
                     throw std::runtime_error("Attempting to dereference NULL smart pointer");
                 }

      // Attributes

      protected: const T *_ptr;
    };

  template <typename T>
    class const_own : public smrt<T>
    {
      typedef const_own<T>  Type_t;
      typedef smrt<T>       Parent_t;

      using Parent_t::_ptr;
      using Parent_t::assert_set;

      // Constructors and Assignment

      public: const_own(const T *p=NULL) : Parent_t(p) {}

      public: Type_t &operator=(const T* p) 
              { 
                if(_ptr != p && _ptr != NULL) delete _ptr;
                _ptr = p;
                return *this;
              }

      public: ~const_own() { if(_ptr != NULL) delete _ptr; }

      public: void release(void) { if(_ptr != NULL) delete _ptr; _ptr = NULL; }
    };

  template <typename T>
    class own : public const_own<T>
    {
      typedef       own<T> Type_t;
      typedef const_own<T> Parent_t;
      typedef      smrt<T> Base_t;

      using Base_t::_ptr;
      using Base_t::assert_set;

      // Constructors and Assignment

      public: own(T *p=NULL) : Parent_t(p) {}

      public: Type_t &operator=(T* p) { Parent_t::operator=(p); return *this; }

      // Methods
      //------------------------------------------------------------
      // Note that although we are casting away the const-ness of _ptr,
      //   all is OK.  Because this is a smrt<T> object, it must have been
      //   constructed with either a non-const T* or another smrt<T>.  The
      //   only reason that _ptr is a const T* is that it is inherited from
      //   the parent smrt<T> class, which requires it to be a const T*
      //------------------------------------------------------------
      public:    T &operator*(void)  const { assert_set(); return *const_cast<T*>(this->_ptr); }
      public:    T *operator->(void) const { assert_set(); return  const_cast<T*>(this->_ptr); }
      public:    T *raw(void)        const {               return  const_cast<T*>(this->_ptr); }
    };


  template <typename T>
    class const_shr : public smrt<T>
    {
      typedef const_shr<T>  Type_t;
      typedef smrt<T>       Parent_t;

      using Parent_t::_ptr;
      using Parent_t::assert_set;

      // Constructors and Assignment

      public: const_shr(const T *p=NULL) : Parent_t(NULL), _refCount(NULL) { set(p); }
      public: const_shr(const Type_t &p) : Parent_t(NULL), _refCount(NULL) { set(p); }

      public: ~const_shr() { decr(); }

      public: void release(void) { set(NULL); }

      public: Type_t &operator=(const T*  p)     { set(p); return *this; }
      public: Type_t &operator=(const Type_t &p) { set(p); return *this; }

      // Public Methods

      public: unsigned long refCount(void) const { return ( _refCount==NULL ? 0UL : *_refCount ); }

      // Internal Methods

      protected: void set(const T* p)
                 {
                   decr();
                   _ptr = p;
                   if(p!=NULL) { _refCount = new unsigned long; *_refCount = 1; }
                   else        { _refCount = NULL;                              }
                 }

      protected: void set(const const_shr<T> &p)
                 {
                   decr();
                   _ptr = p._ptr;
                   if( p.isSet() ) { _refCount = p._refCount; *_refCount += 1; }
                   else            { _refCount = NULL;                         }
                 }

      protected: void decr(void)
                 {
                   if( _refCount != NULL )
                   {
                     *_refCount -= 1;
                     if(*_refCount==0) { delete _ptr; delete _refCount; _ptr = NULL; _refCount = NULL; }
                   }
                 }

      // Attributes

      protected: unsigned long *_refCount;
    };

  template <typename T>
    class shr : public const_shr<T>
    {
      typedef       shr<T> Type_t;
      typedef const_shr<T> Parent_t;
      typedef      smrt<T> Base_t;

      using Base_t::_ptr;
      using Base_t::assert_set;

      // Constructors and Assignment

      public: shr(T *p=NULL)        : Parent_t(p) {}
      public: shr(const Type_t &p)  : Parent_t(p) {}

      public: Type_t &operator=(T*  p)           { Parent_t::set(p); return *this; }
      public: Type_t &operator=(const Type_t &p) { Parent_t::set(p); return *this; }

      // Methods (see notes above in own<T> class)

      public: T &operator*(void)  const { assert_set(); return *const_cast<T*>(this->_ptr); }
      public: T *operator->(void) const { assert_set(); return  const_cast<T*>(this->_ptr); }
      public: T *raw(void)        const {               return  const_cast<T*>(this->_ptr); }
    };


  template <typename T>
    class const_ref : public smrt<T>
    {  
      typedef const_ref<T>  Type_t;
      typedef smrt<T>       Parent_t;

      using Parent_t::_ptr;
      using Parent_t::assert_set;

      // Constructors and Assignement

      public: const_ref(const Parent_t &p) : Parent_t(p) {}

      public: Type_t &operator=( const Parent_t &p ) { Parent_t::operator=(p); return *this; }

      public: void clear(void) { _ptr = NULL; }
    };

  template <typename T>
    class ref : public const_ref<T>
    {
      typedef       ref<T> Type_t;
      typedef const_ref<T> Parent_t;
      typedef      smrt<T> Base_t;

      using Base_t::_ptr;
      using Base_t::assert_set;

      // Constructors and Assignment

      public: ref(const own<T> &p)  : Parent_t(p) {}
      public: ref(const shr<T> &p)  : Parent_t(p) {}
      public: ref(const ref<T> &p)  : Parent_t(p) {}

      public: Type_t &operator=(const own<T> &p) { Parent_t::set(p); return *this; }
      public: Type_t &operator=(const shr<T> &p) { Parent_t::set(p); return *this; }
      public: Type_t &operator=(const ref<T> &p) { Parent_t::set(p); return *this; }

      // Methods (see notes above in own<T> class)

      public: T &operator*(void)  const { assert_set(); return *const_cast<T*>(this->_ptr); }
      public: T *operator->(void) const { assert_set(); return  const_cast<T*>(this->_ptr); }
      public: T *raw(void)        const {               return  const_cast<T*>(this->_ptr); }
    };

#ifdef NS
}
#endif

#endif  // _SMARTPOINTERS_H_
