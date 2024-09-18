# To compile on SunOS: add "-lsocket -lnsl" to LDFLAGS
# To compile with PKCS11: add "-lpkcs11-helper" to LDFLAGS

CFLAGS	?= -O2
WARNING_CFLAGS ?= -Wall -W 
LDFLAGS ?=

LOCAL_CFLAGS = $(WARNING_CFLAGS) -I../include -D_FILE_OFFSET_BITS=64
LOCAL_LDFLAGS = -L../library 			\
		-lmbedtls$(SHARED_SUFFIX)	\
		-lmbedx509$(SHARED_SUFFIX)	\
		-lmbedcrypto$(SHARED_SUFFIX)

ifndef SHARED
DEP=../library/libmbedcrypto.a ../library/libmbedx509.a ../library/libmbedtls.a
else
DEP=../library/libmbedcrypto.$(DLEXT) ../library/libmbedx509.$(DLEXT) ../library/libmbedtls.$(DLEXT)
endif

ifdef DEBUG
LOCAL_CFLAGS += -g3
endif

# if we're running on Windows, build for Windows
ifdef WINDOWS
WINDOWS_BUILD=1
endif

ifdef WINDOWS_BUILD
DLEXT=dll
EXEXT=.exe
LOCAL_LDFLAGS += -lws2_32
ifdef SHARED
SHARED_SUFFIX=.$(DLEXT)
endif
else
DLEXT=so
EXEXT=
SHARED_SUFFIX=
endif

# Zlib shared library extensions:
ifdef ZLIB
LOCAL_LDFLAGS += -lz
endif

APPS = nc$(EXEXT) hv$(EXEXT) hv-cn$(EXEXT)

.SILENT:

.PHONY: all clean list

all: $(APPS)

$(DEP):
	$(MAKE) -C ../library

nc$(EXEXT): nc.c $(DEP)
	echo "  CC    nc.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c nc.c
	$(CC) nc.o -o $@ $(LOCAL_LDFLAGS) $(LDFLAGS)

hv$(EXEXT): hv.c $(DEP)
	echo "  CC    hv.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c hv.c
	$(CC) hv.o -o $@ $(LOCAL_LDFLAGS) $(LDFLAGS)

hv-cn$(EXEXT): hv-cn.c $(DEP)
	echo "  CC    hv-cn.c"
	$(CC) $(LOCAL_CFLAGS) $(CFLAGS) -c hv-cn.c
	$(CC) hv-cn.o -o $@ $(LOCAL_LDFLAGS) $(LDFLAGS)

clean:
ifndef WINDOWS
	rm -f $(APPS)
else
	del /S /Q /F *.o *.exe
endif

list:
	echo $(APPS)
