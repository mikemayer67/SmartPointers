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
      //   True construction must occur in the subclasses

      protected: smrt(void) : _ptr(NULL) {}

      // Methods

      public: const T &operator*(void)  const { validate(); return *_ptr; }
      public: const T *operator->(void) const { validate(); return  _ptr; }
      public: const T *raw(void)        const {             return  _ptr; }

      public: bool isSet(void)     const { return _ptr!=NULL; }
      public: bool isNull(void)    const { return _ptr==NULL; }
      public: bool isNotNull(void) const { return _ptr!=NULL; }

      public: bool operator == (const Type_t &p) const { return _ptr == p._ptr; }
      public: bool operator <  (const Type_t &p) const { return _ptr <  p._ptr; }

      protected: void validate(void) const
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

      // Constructors and Assignment

      public:  const_own(const T *p=NULL) { this->_ptr = p; }

      public: Type_t &operator=(const T* p) 
              { 
                if(this->_ptr != p && this->_ptr != NULL) delete this->_ptr;
                this->_ptr = p;
                return *this;
              }

      private: Type_t &operator=(const Type_t  &p);

      public: ~const_own() { if(this->_ptr != NULL) delete this->_ptr; }

      public: void release(void) { if(this->_ptr != NULL) delete this->_ptr; this->_ptr = NULL; }
    };

  template <typename T>
    class own : public const_own<T>
    {
      typedef       own<T> Type_t;
      typedef const_own<T> Parent_t;
      typedef      smrt<T> Base_t;

      using Base_t::validate;

      // Constructors and Assignment

      public:  own(T *p=NULL) : Parent_t(p) {}

      public:  Type_t &operator=(T* p) { Parent_t::operator=(p); return *this; }
      private: Type_t &operator=(const Type_t  &p);

      // Methods
      //------------------------------------------------------------
      // Note that although we are casting away the const-ness of _ptr,
      //   all is OK.  Because this is a smrt<T> object, it must have been
      //   constructed with either a non-const T* or another smrt<T>.  The
      //   only reason that _ptr is a const T* is that it is inherited from
      //   the parent smrt<T> class, which requires it to be a const T*
      //------------------------------------------------------------
      public:    T &operator*(void)  const { validate(); return *const_cast<T*>(this->_ptr); }
      public:    T *operator->(void) const { validate(); return  const_cast<T*>(this->_ptr); }
      public:    T *raw(void)        const {             return  const_cast<T*>(this->_ptr); }
    };


  template <typename T>
    class const_shr : public smrt<T>
    {
      typedef const_shr<T>  Type_t;
      typedef smrt<T>       Parent_t;

      // Constructors and Assignment

      public: const_shr(const T *p=NULL) : _refCount(NULL) { set(p); }
      public: const_shr(const Type_t &p) : _refCount(NULL) { set(p); }

      public: ~const_shr() { decr(); }

      public: void release(void) { set(NULL); }

      public: Type_t &operator=(const T*  p)     { set(p); return *this; }
      public: Type_t &operator=(const Type_t &p) { set(p); return *this; }

      // Public Methods

      public: unsigned long refCount(void) const { return ( _refCount ? *_refCount : 0UL ); }

      // Internal Methods

      protected: void set(const T* p)
                 {
                   decr();
                   this->_ptr = p;
                   if(p!=NULL) { _refCount = new unsigned long; *_refCount = 1; }
                   else        { _refCount = NULL;                              }
                 }

      protected: void set(const const_shr<T> &p)
                 {
                   decr();
                   this->_ptr = p._ptr;
                   if( p.isSet() ) { _refCount = p._refCount; *_refCount += 1; }
                   else            { _refCount = NULL;                         }
                 }

      protected: void decr(void)
                 {
                   if( _refCount != NULL )
                   {
                     *_refCount -= 1;
                     if(*_refCount==0) { delete this->_ptr; delete _refCount; this->_ptr = NULL; _refCount = NULL; }
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
      using Base_t::validate;

      // Constructors and Assignment

      public: shr(T *p=NULL)        : Parent_t(p) {}
      public: shr(const Type_t &p)  : Parent_t(p) {}

      public: Type_t &operator=(T*  p)           { Parent_t::set(p); return *this; }
      public: Type_t &operator=(const Type_t &p) { Parent_t::set(p); return *this; }

      // Methods (see notes above in own<T> class)

      public: T &operator*(void)  const { validate(); return *const_cast<T*>(this->_ptr); }
      public: T *operator->(void) const { validate(); return  const_cast<T*>(this->_ptr); }
      public: T *raw(void)        const {             return  const_cast<T*>(this->_ptr); }
    };


  template <typename T>
    class const_ref : public smrt<T>
    {  
      typedef const_ref<T>  Type_t;
      typedef smrt<T>       Parent_t;

      // Constructors and Assignement

      public: const_ref(void) {}
      public: const_ref(const Parent_t &p) { this->_ptr = p.raw(); }

      public: Type_t &operator=( const Parent_t &p ) 
              { 
                this->_ptr = p.raw(); 
                return *this; 
              }

      public: void clear(void) { this->_ptr = NULL; }
    };

  template <typename T>
    class ref : public const_ref<T>
    {
      typedef       ref<T> Type_t;
      typedef const_ref<T> Parent_t;
      typedef      smrt<T> Base_t;

      using Base_t::validate;

      // Constructors and Assignment

      public: ref(void) {}
      public: ref(const own<T> &p) : Parent_t(p) {}
      public: ref(const shr<T> &p) : Parent_t(p) {}
      public: ref(const ref<T> &p) : Parent_t(p) {}

      public: Type_t &operator=(const own<T> &p) { Parent_t::operator=(p); return *this; }
      public: Type_t &operator=(const shr<T> &p) { Parent_t::operator=(p); return *this; }
      public: Type_t &operator=(const ref<T> &p) { Parent_t::operator=(p); return *this; }

      // Methods (see notes above in own<T> class)

      public: T &operator*(void)  const { validate(); return *const_cast<T*>(this->_ptr); }
      public: T *operator->(void) const { validate(); return  const_cast<T*>(this->_ptr); }
      public: T *raw(void)        const {             return  const_cast<T*>(this->_ptr); }
    };

#ifdef NS
}
#endif

#endif  // _SMARTPOINTERS_H_
