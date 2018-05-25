#include <iostream>
#include "SmartPointers.h"
#include "test_common.h"

void raw_tests(void)
{
  std::cout << std::endl << "======> raw pointer tests <=======" << std::endl;
  TEST(A *a1 = new A);
  TEST(A *a2 = new B);
  TEST(B *b1 = new B);

  // memory management needed for raw pointers
  TEST(delete a1);
  TEST(delete a2);
  TEST(delete b1);

  std::cout << std::endl << "--DONE--" << std::endl;
}

void const_own_tests(void)
{
  std::cout << std::endl << "======> const_own<T> tests <=======" << std::endl;
  TEST( ns::const_own<A> a1 = new A);
  TEST( ns::const_own<A> a2 = new B);
  TEST( ns::const_own<B> b1 = new B);
  TEST( ns::const_own<B> b2);

  try
  {
    TEST(std::cout << *a1 << std::endl);
    TEST(std::cout << *a2 << std::endl);
    TEST(std::cout << *b1 << std::endl);
    TEST(std::cout << *b2 << std::endl);
  }
  catch(std::runtime_error e)
  {
    std::cout << ">>> runtime error caught: " << e.what() << " <<< " << std::endl;
  }

  TEST(b2 = new B);
  TEST(std::cout << *b2 << std::endl);
  TEST(b2 = new B);
  TEST(std::cout << *b2 << std::endl);

  TEST(const A *ap = a1.raw());
  TEST(std::cout << *ap << std::endl);

  TEST(a1->const_func());
  TEST(b1->const_func());

  TEST(b1.release());

  std::cout << std::endl << "--DONE--" << std::endl;
}

void own_tests(void)
{
  std::cout << std::endl << "======> own<T> tests <=======" << std::endl;
  TEST( ns::own<A> a1 = new A);
  TEST( ns::own<A> a2 = new B);
  TEST( ns::own<B> b1 = new B);
  TEST( ns::own<B> b2);

  try
  {
    TEST(std::cout << *a1 << std::endl);
    TEST(std::cout << *a2 << std::endl);
    TEST(std::cout << *b1 << std::endl);
    TEST(std::cout << *b2 << std::endl);
  }
  catch(std::runtime_error e)
  {
    std::cout << ">>> runtime error caught: " << e.what() << " <<< " << std::endl;
  }
  TEST(b2 = new B);
  TEST(std::cout << *b2 << std::endl);
  TEST(b2 = new B);
  TEST(std::cout << *b2 << std::endl);

  TEST(const A *ap = a1.raw());
  TEST(std::cout << *ap << std::endl);

  TEST(a1->func());
  TEST(a1->const_func());
  TEST(b1->func());
  TEST(b1->const_func());

  TEST(b1.release());

  std::cout << std::endl << "--DONE--" << std::endl;
}

void const_shr_tests(void)
{
  std::cout << std::endl << "======> const_shr<T> tests <=======" << std::endl;
  TEST( ns::const_shr<A> a1 = new A);
  TEST( ns::const_shr<A> a2 = new B);
  TEST( ns::const_shr<B> b1 = new B);
  TEST( ns::const_shr<B> b2);

  try
  {
    TEST(std::cout << *a1 << std::endl);
    TEST(std::cout << *a2 << std::endl);
    TEST(std::cout << *b1 << std::endl);
    TEST(std::cout << *b2 << std::endl);
  }
  catch(std::runtime_error e)
  {
    std::cout << ">>> runtime error caught: " << e.what() << " <<< " << std::endl;
  }
  TEST(b2=b1);
  TEST(std::cout << *b2 << std::endl);

  TEST(const A *ap = a1.raw());
  TEST(std::cout << *ap << std::endl);
  TEST(a1=a2);
  SHOW_SHR(a1);

//  TEST(a1->func());  // will rightfully fail compilation if uncommented
//  TEST(b1->func());  // will rightfully fail compilation if uncommented
  TEST(a1->const_func());
  TEST(b1->const_func());

  TEST(a2.release());
  SHOW_SHR(a1);

  std::cout << std::endl << "--DONE--" << std::endl;
}

void shr_tests(void)
{
  std::cout << std::endl << "======> shr<T> tests <=======" << std::endl;
  TEST( ns::shr<A> a1 = new A);
  TEST( ns::shr<A> a2 = new B);
  TEST( ns::shr<B> b1 = new B);
  TEST( ns::shr<B> b2);

  try
  {
    TEST(std::cout << *a1 << std::endl);
    TEST(std::cout << *a2 << std::endl);
    TEST(std::cout << *b1 << std::endl);
    TEST(std::cout << *b2 << std::endl);
  }
  catch(std::runtime_error e)
  {
    std::cout << ">>> runtime error caught: " << e.what() << " <<< " << std::endl;
  }
  TEST(b2=b1);
  TEST(std::cout << *b2 << std::endl);

  TEST(const A *ap = a1.raw());
  TEST(std::cout << *ap << std::endl);
  TEST(a1=a2);
  SHOW_SHR(a1);

  TEST(a1->func());
  TEST(a1->const_func());
  TEST(b1->func());
  TEST(b1->const_func());

  TEST(a2.release());
  SHOW_SHR(a1);
  SHOW_SHR(a2);

  std::cout << std::endl << "--DONE--" << std::endl;
}

void cross_tests(void)
{
  std::cout << std::endl << "======> shr<T> / const_shr<T> tests <=======" << std::endl;

  TEST(ns::const_shr<A> ca1 = new A);
  TEST(ns::const_shr<A> ca2 = ca1);

  TEST(ns::shr<A> a1 = new A);
  TEST(ns::shr<A> a2 = a1);

//  TEST(shr<A> a3 = ca1);  // will rightfully fail compilation if uncommented
  TEST(ns::const_shr<A> ca3 = a1);

  TEST(a1->const_func());
  TEST(a1->func());
  TEST(ca3->const_func());
//  TEST(ca3->func());  // will rightfully fail compilation if uncommented (even though same pointer as a1)

  TEST( if(a1==ca1) std::cout << "  OK"; else std::cout << "NOPE"; std::cout << ":  " << *a1 << " vs " << *ca1 << std::endl); 
  TEST( if(a1==ca3) std::cout << "  OK"; else std::cout << "NOPE"; std::cout << ":  " << *a1 << " vs " << *ca3 << std::endl);

  TEST( A* ra1 = a1.raw() );
//  TEST( A* ra2 = ca1.raw() );  // will rightfully fail compilation if uncommented  

  TEST( const A* cra1 = a1.raw() );
  TEST( const A* cra2 = ca1.raw() );

  TEST(ca1=a1);
  TEST(ca2=a1);
  TEST(ca3=a1);

  std::cout << std::endl << "--DONE--" << std::endl;
}

void const_ref_tests(void)
{
  std::cout << std::endl << "======> const_ref<T> tests <=======" << std::endl;
  TEST( ns::const_own<A> oa1 = new A);
  TEST(       ns::own<A> oa2 = new A);
  TEST( ns::const_own<B> ob1 = new B);
  TEST( ns::const_shr<A> sa1 = new A);
  TEST( ns::const_shr<A> sa2 = sa1  );
  TEST(       ns::shr<A> sa3 = new A);
  TEST( ns::const_shr<B> sb1 = new B);
  TEST( ns::const_ref<A>  a1 = oa1  );
  TEST( ns::const_ref<A>  a2 = sa1  );
  TEST( ns::const_ref<A>  a3 = sa2  );
  TEST( ns::const_ref<A>  a4 = oa2  );
  TEST( ns::const_ref<A>  a5 = sa3  );
  TEST( ns::const_ref<B>  b1 = ob1  );
  TEST( ns::const_ref<B>  b2 = sb1  );

  try
  {
    TEST(std::cout << *a1 << std::endl);
    TEST(std::cout << *a2 << std::endl);
    TEST(std::cout << *a3 << std::endl);
    TEST(std::cout << *a4 << std::endl);
    TEST(std::cout << *a5 << std::endl);
    TEST(std::cout << *b1 << std::endl);
    TEST(std::cout << *b2 << std::endl);
  }
  catch(std::runtime_error e)
  {
    std::cout << ">>> runtime error caught: " << e.what() << " <<< " << std::endl;
  }

  TEST(const A *ap = a1.raw());
  TEST(std::cout << *ap << std::endl);

  TEST(a1->const_func());
  TEST(b1->const_func());

  try
  {
    SHOW_SHR(sa1);
    SHOW_SHR(sa2);
    TEST(sa1.release());
    SHOW_SHR(sa1);
    SHOW_SHR(sa2);
    TEST(sa2.release());
    SHOW_SHR(sa1);
    SHOW_SHR(sa2);
    TEST(std::cout << *a2 << std::endl);
  }
  catch(std::runtime_error e)
  {
    std::cout << ">>> runtime error caught: " << e.what() << " <<< " << std::endl;
  }

  std::cout << std::endl << "--DONE--" << std::endl;
}

void ref_tests(void)
{
  std::cout << std::endl << "======> ref<T> tests <=======" << std::endl;
  TEST( ns::own<A> oa1 = new A);
  TEST( ns::own<B> ob1 = new B);
  TEST( ns::shr<A> sa1 = new A);
  TEST( ns::shr<A> sa2 = sa1);
  TEST( ns::shr<B> sb1 = new B);
  TEST( ns::ref<A>  a1 = oa1 );
  TEST( ns::ref<A>  a2 = sa1 );
  TEST( ns::ref<A>  a3 = sa2 );
  TEST( ns::ref<B>  b1 = ob1 );
  TEST( ns::ref<B>  b2 = sb1 );

  try
  {
    TEST(std::cout << *a1 << std::endl);
    TEST(std::cout << *a2 << std::endl);
    TEST(std::cout << *a3 << std::endl);
    TEST(std::cout << *b1 << std::endl);
    TEST(std::cout << *b2 << std::endl);
  }
  catch(std::runtime_error e)
  {
    std::cout << ">>> runtime error caught: " << e.what() << " <<< " << std::endl;
  }

  TEST(A *ap = a1.raw());
  TEST(std::cout << *ap << std::endl);

  TEST(a1->const_func());
  TEST(b1->const_func());

  TEST(a1->func());
  TEST(b1->func());

  try
  {
    SHOW_SHR(sa1);
    SHOW_SHR(sa2);
    TEST(sa1.release());
    SHOW_SHR(sa1);
    SHOW_SHR(sa2);
    TEST(sa2.release());
    SHOW_SHR(sa1);
    SHOW_SHR(sa2);
    TEST(std::cout << *a2 << std::endl);
  }
  catch(std::runtime_error e)
  {
    std::cout << ">>> runtime error caught: " << e.what() << " <<< " << std::endl;
  }

  std::cout << std::endl << "--DONE--" << std::endl;
}

int main(int argc,const char **argv)
{
  raw_tests();
  const_own_tests();
  own_tests();
  const_shr_tests();
  shr_tests();
  cross_tests();
  const_ref_tests();
  ref_tests();

  return 0;      
}
