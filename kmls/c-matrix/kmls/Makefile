# based on matrixssl's app/dtls

MATRIXSSL_ROOT:=../..

NC_SRC:=nc.c
NC_EXE:=nc$(E)

HV_SRC:=hv.c
HV_EXE:=hv$(E)

HV-CN_SRC:=hv-cn.c
HV-CN_EXE:=hv-cn$(E)

# SRC:=$(NC_SRC)

include $(MATRIXSSL_ROOT)/common.mk

# Linked files
STATIC:=\
	$(MATRIXSSL_ROOT)/matrixssl/libssl_s.a \
	$(MATRIXSSL_ROOT)/crypto/libcrypt_s.a \
	$(MATRIXSSL_ROOT)/core/libcore_s.a

CIPHER_OPTION=ID_RSA # The default cipher option
CFLAGS+=-D$(CIPHER_OPTION)

all: compile

compile: $(OBJS) $(HV_EXE) $(HV-CN_EXE)

# Additional Dependencies
$(OBJS): $(MATRIXSSL_ROOT)/common.mk Makefile $(wildcard *.h)

$(NC_EXE): $(NC_SRC:.c=.o) $(STATIC)
	$(CC) -o $@ $^ $(LDFLAGS)

$(HV_EXE): $(HV_SRC:.c=.o) $(STATIC)
	$(CC) -o $@ $^ $(LDFLAGS)	

$(HV-CN_EXE): $(HV-CN_SRC:.c=.o) $(STATIC)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(HV_EXE) $(HV-CN_EXE) $(OBJS)

