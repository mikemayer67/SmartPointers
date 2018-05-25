CC = g++
RM = rm -rf

TARGETS = test_global test_sp test_ns test_stl

all: $(TARGETS)

test_global : ../SmartPointers.h test_common.h test_global.cc Makefile
	$(CC) -I.. -g -o test_global test_global.cc

test_sp : ../SmartPointers.h test_common.h test_sp.cc Makefile
	$(CC) -I.. -g -DSMARTPOINTER_NAMESPACE -o test_sp test_sp.cc

test_ns : ../SmartPointers.h test_common.h test_ns.cc Makefile
	$(CC) -I.. -g -DSMARTPOINTER_NAMESPACE=ns -o test_ns test_ns.cc

test_stl : ../SmartPointers.h test_common.h test_stl.cc Makefile
	$(CC) -I.. -g -o test_stl test_stl.cc

clean: 
	$(RM) *.o *~

clobber: clean
	$(RM) $(TARGETS)
