CFLAGS	?= -O2
WARNING_CFLAGS ?= -Wall -W 
LDFLAGS ?=

LOCAL_CFLAGS = -I../build/include

LOCAL_LDFLAGS = -L../build/lib/ -lwolfssl -L$(KLEE_LIB) -lkleeRuntest -lm

APPS = hv hv-cn

.PHONY: all clean

all: $(APPS)

hv$(EXEXT): hv.c
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c hv.c
	$(CC) hv.o -o $@ $(LOCAL_LDFLAGS) $(LDFLAGS)

hv-cn$(EXEXT): hv-cn.c
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c hv-cn.c
	$(CC) hv-cn.o -o $@ $(LOCAL_LDFLAGS) $(LDFLAGS)

clean:
	rm -f $(APPS) *.o *.bc
