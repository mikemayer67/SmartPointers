#include <iostream>
#include <vector>
#include <set>

#include "SmartPointers.h"
#include "test_common.h"

using namespace std;

void vector_test(void)
{
  cout << endl << "----------Vector tests-----------" << endl << endl;

  cout << "_build alist_";
  TEST(vector< shr<A> > alist);

  TEST( alist.push_back( new A ) );
  TEST( alist.push_back( new A ) );
  TEST( alist.push_back( new B ) );
  TEST( alist.push_back( new A ) );

  cout << endl << "_build alist2_";
  TEST(vector< shr<A> > alist2);

  TEST( alist2.push_back( new A ) );
  TEST( alist2.push_back( new A ) );
  TEST( alist2.push_back( new B ) );
  TEST( alist2.push_back( new A ) );

  cout << endl << "_build calist_";
  TEST(vector< const_shr<A> > calist);

  TEST( calist.push_back( new A ) );
  TEST( calist.push_back( new A ) );
  TEST( calist.push_back( new B ) );
  TEST( calist.push_back( new A ) );

  cout << endl << "_insert alist2 content to end of alist_";
  TEST( alist.insert(alist.end(),alist2.begin(),alist2.end()) );

  cout << endl << "_copy alist content to end of calist_";
  TEST( calist.insert(calist.begin(), alist.begin(), alist.end() ) );
  for(vector< shr<A> >::iterator x=alist.begin(); x!=alist.end(); ++x)
  {
    calist.push_back( *x);
  }

  cout << endl << "_display calist_" << endl;
  int i=0;
  for(vector< const_shr<A> >::iterator x=calist.begin(); x!=calist.end(); ++x)
  {
    const_shr<A> t = *x;
    cout << "calist[" << i << "] = " << *t << "  refCount=" << x->refCount() << "  ptr=" << std::hex << long(x->raw()) << endl;
  }

  cout << endl << "_empty calist_";
  TEST( calist.clear() );

  cout << endl << "DONE" << endl;
}

void set_test(void)
{
  cout << endl << "----------Set tests-----------" << endl << endl;

  cout << "_build alist_";
  TEST(set< shr<A> > alist);

  TEST( alist.insert( new A ) );
  TEST( alist.insert( new A ) );
  TEST( alist.insert( new B ) );
  TEST( alist.insert( new A ) );

  cout << endl << "_build alist2_";
  TEST(set< shr<A> > alist2);

  TEST( alist2.insert( new A ) );
  TEST( alist2.insert( new A ) );
  TEST( alist2.insert( new B ) );
  TEST( alist2.insert( new A ) );

  cout << endl << "_build calist_";
  TEST(set< const_shr<A> > calist);

  TEST( calist.insert( new A ) );
  TEST( calist.insert( new A ) );
  TEST( calist.insert( new B ) );
  TEST( calist.insert( new A ) );

  cout << endl << "_insert alist2 content to end of alist_";
  TEST( alist.insert(alist2.begin(),alist2.end()) );

  cout << endl << "_copy alist content to end of calist_";
  TEST( calist.insert(alist.begin(), alist.end() ) );
  for(set< shr<A> >::iterator x=alist.begin(); x!=alist.end(); ++x)
  {
    calist.insert( *x);
  }

  cout << endl << "_display calist_" << endl;
  int i=0;
  for(set< const_shr<A> >::iterator x=calist.begin(); x!=calist.end(); ++x)
  {
    const_shr<A> t = *x;
    cout << "calist[" << i << "] = " << *t << "  refCount=" << x->refCount() << "  ptr=" << std::hex<< long(x->raw()) << endl;
  }

  cout << endl << "_empty calist_";
  TEST( calist.clear() );

  cout << endl << "DONE" << endl;
}
int main(int argc,const char **argv)
{
  vector_test();
  set_test();
  return 0;
}


