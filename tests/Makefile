CC = g++
RM = rm -rf

TARGETS = test_global test_sp test_ns test_stl

all: $(TARGETS)

test_global : ../SmartPointers.h test_common.h test_global.cc
	$(CC) -I.. -o test_global test_global.cc

test_sp : ../SmartPointers.h test_common.h test_sp.cc
	$(CC) -I.. -DSMARTPOINTER_NAMESPACE -o test_sp test_sp.cc

test_ns : ../SmartPointers.h test_common.h test_ns.cc
	$(CC) -I.. -DSMARTPOINTER_NAMESPACE=ns -o test_ns test_ns.cc

test_stl : ../SmartPointers.h test_common.h test_stl.cc
	$(CC) -I.. -o test_stl test_stl.cc

clean: 
	$(RM) *.o *~

clobber: clean
	$(RM) $(TARGETS)
