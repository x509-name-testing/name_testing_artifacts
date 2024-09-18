all : nc

STRONGSWAN_HOME=../../..
LIBSTRONGSWAN_HOME=..
LIBS=$(LIBSTRONGSWAN_HOME)/.libs


CFLAGS+= -include $(STRONGSWAN_HOME)/config.h
CFLAGS+= -I$(LIBSTRONGSWAN_HOME)

nc: nc.c $(LIBS)/libstrongswan.a $(GMP_LIBS)/libgmp.a
	$(CC) $(CFLAGS) -g nc.c -o $@ $(LDFLAGS) $(LIBS)/libstrongswan.a -lpthread -ldl $(GMP_LIBS)/libgmp.a

clean:
	rm -rf nc *.bc
