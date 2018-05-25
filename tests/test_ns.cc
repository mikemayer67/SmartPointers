#include <iostream>
#include "SmartPointers.h"
#include "test_common.h"

using namespace std;

void raw_tests(void)
{
  cout << endl << "======> raw pointer tests <=======" << endl;
  TEST(A *a1 = new A);
  TEST(A *a2 = new B);
  TEST(B *b1 = new B);

  // memory management needed for raw pointers
  TEST(delete a1);
  TEST(delete a2);
  TEST(delete b1);

  cout << endl << "--DONE--" << endl;
}

void const_shr_tests(void)
{
  cout << endl << "======> ns::const_shr<T> tests <=======" << endl;
  TEST( ns::const_shr<A> a1 = new A);
  TEST( ns::const_shr<A> a2 = new B);
  TEST( ns::const_shr<B> b1 = new B);
  TEST( ns::const_shr<B> b2);

  try
  {
    TEST(cout << *a1 << endl);
    TEST(cout << *a2 << endl);
    TEST(cout << *b1 << endl);
    TEST(cout << *b2 << endl);
  }
  catch(std::runtime_error e)
  {
    cout << ">>> runtime error caught: " << e.what() << " <<< " << endl;
  }
  TEST(b2=b1);
  TEST(cout << *b2 << endl);

  TEST(const A *ap = a1.raw());
  TEST(cout << *ap << endl);
  TEST(a1=a2);
  TEST(cout << *a1 << "  refCount=" << a1.refCount() << std::endl);

//  TEST(a1->func());  // will rightfully fail compilation if uncommented
//  TEST(b1->func());  // will rightfully fail compilation if uncommented
  TEST(a1->const_func());
  TEST(b1->const_func());

  cout << endl << "--DONE--" << endl;
}

void shr_tests(void)
{
  cout << endl << "======> ns::shr<T> tests <=======" << endl;
  TEST( ns::shr<A> a1 = new A);
  TEST( ns::shr<A> a2 = new B);
  TEST( ns::shr<B> b1 = new B);
  TEST( ns::shr<B> b2);

  try
  {
    TEST(cout << *a1 << endl);
    TEST(cout << *a2 << endl);
    TEST(cout << *b1 << endl);
    TEST(cout << *b2 << endl);
  }
  catch(std::runtime_error e)
  {
    cout << ">>> runtime error caught: " << e.what() << " <<< " << endl;
  }
  TEST(b2=b1);
  TEST(cout << *b2 << endl);

  TEST(const A *ap = a1.raw());
  TEST(cout << *ap << endl);
  TEST(a1=a2);
  TEST(cout << *a1 << "  refCount=" << a1.refCount() << std::endl);

  TEST(a1->func());
  TEST(a1->const_func());
  TEST(b1->func());
  TEST(b1->const_func());
  cout << endl << "--DONE--" << endl;
}

void cross_tests(void)
{
  cout << endl << "======> ns::shr<T> / ns::const_shr<T> tests <=======" << endl;

  TEST(ns::const_shr<A> ca1 = new A);
  TEST(ns::const_shr<A> ca2 = ca1);

  TEST(ns::shr<A> a1 = new A);
  TEST(ns::shr<A> a2 = a1);

//  TEST(ns::shr<A> a3 = ca1);  // will rightfully fail compilation if uncommented
  TEST(ns::const_shr<A> ca3 = a1);

  TEST(a1->const_func());
  TEST(a1->func());
  TEST(ca3->const_func());
//  TEST(ca3->func());  // will rightfully fail compilation if uncommented (even though same pointer as a1)

  TEST( if(a1==ca1) cout << "  OK"; else cout << "NOPE"; cout << ":  " << *a1 << " vs " << *ca1 << endl); 
  TEST( if(a1==ca3) cout << "  OK"; else cout << "NOPE"; cout << ":  " << *a1 << " vs " << *ca3 << endl);

  TEST( A* ra1 = a1.raw() );
//  TEST( A* ra2 = ca1.raw() );  // will rightfully fail compilation if uncommented  

  TEST( const A* cra1 = a1.raw() );
  TEST( const A* cra2 = ca1.raw() );

  TEST(ca1=a1);
  TEST(ca2=a1);
  TEST(ca3=a1);

  cout << endl << "--DONE--" << endl;
}

int main(int argc,const char **argv)
{
  raw_tests();
  const_shr_tests();
  shr_tests();
  cross_tests();

  return 0;      
}
