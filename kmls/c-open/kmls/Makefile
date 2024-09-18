# OPENSSL_HOME=openssl-1.1.1t
OPENSSL_HOME=$(realpath ..)
INC=-I${OPENSSL_HOME}/include/
CC=wllvm

.PHONY: clean

all: nc hv hv-cn 

nc: 
	$(CC) $@.c $(INC) $(OPENSSL_HOME)/libcrypto.a -ldl $(CFLAGS) -L$(KLEE_LIB) -lkleeRuntest -I$(KLEE_INCLUDE) -o nc

hv:
	$(CC) $@.c $(INC) $(OPENSSL_HOME)/libcrypto.a -ldl $(CFLAGS) -L$(KLEE_LIB) -lkleeRuntest -I$(KLEE_INCLUDE) -o hv

hv-cn:
	$(CC) $@.c $(INC) $(OPENSSL_HOME)/libcrypto.a -ldl $(CFLAGS) -L$(KLEE_LIB) -lkleeRuntest -I$(KLEE_INCLUDE) -o hv-cn

clean: 
	rm -rf *.o nv hv hv-cn *.bc
