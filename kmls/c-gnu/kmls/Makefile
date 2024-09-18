CFLAGS  ?= -O0
WARNING_CFLAGS ?= -Wall -W

# LOCAL_CFLAGS = -I$(GNUDIR)/build/include -I$(GMPDIR)/build/include -I$(NETDIR)/build/include -I$(IDN2DIR)/build/include
LOCAL_CFLAGS = -I$(GNUDIR)/build/include -I$(GMPDIR)/build/include -I$(NETDIR)/build/include

# LOCAL_LDFLAGS = -L$(GNUDIR)/build/lib -lgnutls -L$(NETDIR)/build/lib64 -lhogweed -lnettle -L$(GMPDIR)/build/lib -lgmp -static
# LOCAL_LDFLAGS = $(GNUDIR)/build/lib/libgnutls.a  $(NETDIR)/build/lib64/libhogweed.a  $(NETDIR)/build/lib64/libnettle.a  $(GMPDIR)/build/lib/libgmp.a $(IDN2DIR)/build/lib/libidn2.a
LOCAL_LDFLAGS = $(GNUDIR)/build/lib/libgnutls.a  $(NETDIR)/build/lib64/libhogweed.a  $(NETDIR)/build/lib64/libnettle.a  $(GMPDIR)/build/lib/libgmp.a
APPS = hv hv-cn
# APPS = test

.PHONY: all clean

all: $(APPS)

nc$(EXEXT): nc.c
	# echo "  CC    nc.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c nc.c
	$(CC) nc.o -o $@ $(LOCAL_LDFLAGS) $(LDFLAGS) $(LOCAL_CFLAGS) $(CFLAGS)

hv$(EXEXT): hv.c
	# echo "  CC    hv.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c hv.c
	$(CC) hv.o -o $@ $(LOCAL_LDFLAGS) $(LDFLAGS) $(LOCAL_CFLAGS) $(CFLAGS)

hv-cn$(EXEXT): hv-cn.c
	# echo "  CC    hv-cn.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c hv-cn.c
	$(CC) hv-cn.o -o $@ $(LOCAL_LDFLAGS) $(LDFLAGS) $(LOCAL_CFLAGS) $(CFLAGS)

clean:
	rm -f $(APPS) ./*.o ./hv.bc ./hv-cn.bc
