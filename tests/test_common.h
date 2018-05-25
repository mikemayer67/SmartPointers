#include <iostream>
#include <sstream>

class A
{
  public:
    A(void) { static int nextIndex(0); _a = ++nextIndex; write(std::cout,1); }

    virtual ~A() { write(std::cout,2); }

    void write(std::ostream &s, int status=0) const 
    { 
      std::stringstream key;
      key << "A(" << _a << ")";
      switch(status)
      {
        case 1: s << "Creating: " << key.str() << std::endl; break;
        case 2: s << "Deleting: " << key.str() << std::endl; break;
        default: s << key.str(); break;
      }
    }

    virtual void const_func(void) const 
    { 
      std::cout << "A::const_func(" << _a << ")" << std::endl; 
    }
    virtual void func(void) 
    { 
      std::cout << "A::func(" << _a << ")" << std::endl; 
    }

  protected:
    int _a;
};

class B : public A
{
  public:
    B(void) { static int nextIndex(0); _b = ++nextIndex; write(std::cout,1); }

    virtual ~B() { write(std::cout,2); }

    void write(std::ostream &s, int status=0) const 
    { 
      std::stringstream key;
      key << "B(" << _b << ";" << _a << ")";
      switch(status)
      {
        case 1: s << "Creating: " << key.str() << std::endl; break;
        case 2: s << "Deleting: " << key.str() << std::endl; break;
        default: s << key.str();  break;
      }
    }

    virtual void const_func(void) const 
    { 
      A::const_func(); 
      std::cout << "B::const_func(" <<_b << ";" << _a << ")" << std::endl; 
    }
    virtual void func(void) 
    {
      A::func(); 
      std::cout << "B::func(" <<_b << ";" << _a << ")" << std::endl; 
    }

  protected:
    int _b;
};

std::ostream &operator<<(std::ostream &s, const A &x) { x.write(s); return s; }
std::ostream &operator<<(std::ostream &s, const B &x) { x.write(s); return s; }

#define TEST(x) std::cout << std::endl << "test> " #x << std::endl; x

#define SHOW_SHR(x) \
  std::cout << std::endl << "show> " #x << ": "; \
  if( x.isNull() ) { std::cout << "NULL"; } \
  else             { std::cout << *(x); } \
  std::cout << "  refCount=" << x.refCount() << std::endl;
