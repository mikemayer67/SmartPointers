C++ header file for creating a family of simple, yet powerful smart pointers

-----------------------------------------------------------------------------
History:

  The project was formerly named SharePointer, which is still available on
  GitHub at https://github.com/vmwishes/SharePointer and is still a fully
  viable solution for many smart pointer solutions.

  Unfortunately, SharePointer is inherently not thread safe.  And while
  the share pointer provided in this package is also not thread safe,
  the new pointer types introduced here can be used in a multi-threaded
  environment as long as their memory management rules are understood.

-----------------------------------------------------------------------------
Description:

  This package contains a single header file which defines four smart
  pointer templates (eight, if you count the const and non-const variants
  as distinct):

    smrt<T>                  :  abstract base class for all smart pointers
    own<T>  & const_own<T>   :  exclusively manage pointer memory
    shr<T>  & const_shr<T>   :  cooperatively manage pointer memory
    ref<T>  & const_ref<T>   :  provides no pointer memory management

-----------------------------------------------------------------------------
Const Pointers:

  Before describing each of these in detail, a quick note about the
  const vs non-const variants.  One option for storing a pointer to a
  const object would be to simply put const in the template qualifier,
  e.g.  own<T> vs own<const T>.  The problem with this is that these
  are different template types.  The compiler will not allow assignnment
  from a non-const to a const smart pointer. The const variants of
  the templates allow this assignment to occur.

           T  *p = new T();
     const T *cp = p;       // perfectly valid C++

     own<T>        op = p;
     own<const T> ocp = op;  // invalid, different template types
     const_own<T> cop = op;  // valid,   same template type

   Note that const_shr<T> protects the content of T.  It does not prevent
   you from reassigning the const_shr<T> to point to a different T.

                           Modify T content    Modify which T is referenced
                           ----------------    ----------------------------
     shr<T>                       YES                      YES
     const_shr<T>                  NO                      YES
     const shr<T>                 YES                       NO
     const const_shr<T>            NO                       NO

   Here are some of the key differences:
     - a const_shr<T> can invoke const methods on T, a shr<T> cannot
     - a const_shr<T> cannot modify any of T's attributes unless marked mutable
     - a shr<T> can modify any of T's (public) attributes not marked const
     - the raw() method on a const_shr<T> returns a const T* pointer
     - the raw() method on a shr<T> returns a T* pointer

--------------------------------------------------------------------------------
Smart Pointer Templates:

  smrt<T> is the abstract base class for all smart pointers.  It provides
     utility methods commont to all of the smart pointer types.  It provides
     not memory management and cannot be instantiated directly.

  own<T> is the smart pointer which "owns" the pointer.  It will handle
     freeing the memory when the own<T> instance goes out of scope OR
     is assigned a new pointer to manage OR is epxlicitly told to 
     delete the pointer.

  shr<T> is the smart pointer which shares ownership of the pointer.
     It will handle freeing the memory when the last shr<T> instance
     referencing the memory goes out of scope OR is assigned a new
     pointer to manage OR is explicity told to release ownership
     of the pointer.

  ref<T> is the smart pointer which claims absolutely no ownership
     of the pointer.  It can be used to refrence a pointer owned
     by a shr<T> or an own<T> while providing the smrt<T> pointer
     utility methods.  Care must be taken when using ref<T> pointers
     as they have no way of knowing if/when the own<T> or shr<T>
     owner(s) frees the memory.  ref<T> pointers should never be
     used where they can outlive the owner.

  The smart pointer template heirarchy is structured as follows:

         smrt<T> --<+---- const_own<T> --<|-- own<T>
                    |
                    +---- const_shr<T> --<|-- shr<T>
                    | 
                    +---- const_ref<T> --<|-- ref<T>

   The following list shows the allowable constructors/assignemnts

          own<T>        <=    T*
          const_own<T>  <=    T*  

          shr<T>        <=    T*
          shr<T>        <=    shr<T>
          const_shr<T>  <=    T*  
          const_shr<T>  <=    shr<T>
          const_shr<T>  <=    const_shr<T>

          ref<T>        <=    own<T>
          ref<T>        <=    shr<T>
          ref<T>        <=    ref<T>
          const_ref<T>  <=    own<T>
          const_ref<T>  <=    shr<T>
          const_ref<T>  <=    ref<T>
          const_ref<T>  <=    const_own<T>
          const_ref<T>  <=    const_shr<T>
          const_ref<T>  <=    const_ref<T>

     Note that ref<T> cannot be constructed from or assigned a raw
     pointer.  This is to avoid confusion arising from the need to
     still manage pointer memory outside of the smart pointer constructs.

--------------------------------------------------------------------------------
Memory Management Rules:

  It is still possible to "break" smart pointer memory management.  
    Follow these simple rules and everything should be ok.
    Ignore them and risk segmentation or other memory faults.

    - do not assign a raw pointer to more than one own<T> or shr<T> instance
    - do not delete any raw pointer has been assigned to an own<T> or shr<T>
    - do not delete any raw pointer has been returned using the raw() method
    - do not delete any raw pointer returned using the * dereferencing operator
 

-----------------------------------------------------------------------------
Pubic Methods:

  Constructors and assignement operators are outlined above.

  Dereferencing:
    All smart pointers can be used with both the * and -> operators (as though the 
      smart pointer was the underlying raw pointer).  In addition, smart pointers
      provide the raw() method which returns the underlying raw pointer. Use the
      raw() method with cuation---NEVER deallocate the returned value or pass it
      to a function/method that could deallocate it.

      Note that the . operator applies to the smart pointer instance itself, NOT
      the underlying T pointer.

      x = t1->getXValue();     // invokes the T::getXValue method
      x = t1->x_value;         // accesses the T::x_value property

      x = (*t1).getXValue();   // same as t1->getXValue()
      x = (*t1).x_value;       // same as t1->x_value 

      T& tr = *t1;             // tr is a reference to the T to which t1 points
      T* tp = &(*t1);          // tp is raw pointer to the T to which t1 points
                               //   CAUTION:: Do NOT deallocate tp or pass it
                               //             to any function/method that will

      T* tp = t1.raw();        // tp is raw pointer to the T to which t1 points
                               //   CAUTION:: Do NOT deallocate tp or pass it
                               //             to any function/method that will

  Determining State:
    The following methods can be used to test the state of a smart pointer

      bool isSet()             // returns true unless pointing to NULL
      bool isNull()            // returns true   if   pointing to NULL
      bool isNotNull()         // returns true unless pointing to NULL
                               //   (yes, this is the same as isSet() )

      In addition, the == operator can be used to determine if two smart
        pointers reference the same T*.  The smart pointers need not be
        the same type (but must both template the same type T)

      Finally, the < operator can be used to determine the sorting
        order of two pointers.  Again, the smart pointers need not be the
        same type.  This is included to be used with STL container ordering.

  
  ref<T> only:

      void clear();            // sets pointer to NULL
       
  own<T> only:

      void release();           // sets pointer to NULL, deallocating current pointer.
       
  shr<T> only:

      void release();          // sets pointer to NULL, deallocating current
                                  pointer if this is the last shr<T> pointing to it.
     
      unsigned long refCount() // returns number shr<T> or const_shr<T> instances
                                  sharing the pointer.  Returns 0 if pointing to NULL.

  Exceptions:
    Any attempt to derefence a NULL smart pointer will result in a 
      std::runtime_error exception being thrown.
  
--------------------------------------------------------------------------------
Notes on shr<T> (and const_shr<T>)

 When it comes to reference tracking, it always comes down to 3 options:
   1) keep a counter in the wrapper class
   2) require that the referenced object hold counter
   3) insert a wrapper around the object to hold the counter

   Option 1 is problematic because you must take care when it comes
     to "forking" or "chaining" the smart pointer: e.g.
       shr<T> t1 = new T;    // t1.count=1
       shr<T> t2 = t1;       // t1.count=2 t2.count=2
       shr<T> t3 = t2;       // t1.count=2 t2.count=3 t3.count=3
       shr<T> t4 = t1;       // t1.count=3 t2.count=3 t3.count=3 t4.count=3

   Option 2 is limiting because it only works with classes defined to
     work with your smart pointer.  It does not work with primitive 
     types or classes you have no control over (or do not wish to perturb).

   Option 3 solves the problems with options 1 and 2, but introduces
     a new layer of overhead which could, in some circumstances, become
     a performance hit.

 The share pointer implemented in this package takes a slightly modified
   approach to option 1.  But, rather than storing the reference count, 
   it stores a reference (actually a pointer) to the reference count.
   Thus whenever a new shr<T> is constructed from an existing one
   (or assigned to from another shr<T>), the shared reference count
   is updated and ALL shr<T> pointing to the same memory have access
   to the new count.

 Examples:

     shr<T> t1 = new T;
     shr<T> t2 = t1;
     shr<T> t3(new T);
     shr<T> t4(t3);

     t1 and t2 both point to the same T pointer and have a reference count of 2
     t3 and t4 both point to antother T pointer and have a reference count of 2

     t3 = t1;    // t1, t2, & t3 all point to same T and have reference count of 3
                 // t4 now has a reference count of 1

     t4 = new T; // old T object pointed to by t4 is deallocated
                 // t4 now points to a new T with a reference count of 1

