BEARSSL = ..
BUILD = ${BEARSSL}/bclang
# BUILD = ${BEARSSL}/build
TOOLS = ${BEARSSL}/tools
INC = -I${BEARSSL}/inc -I${TOOLS}
OBJDIR = ${BUILD}/obj
P=/
O=.o

OBJBRSSL = \
 $(OBJDIR)$Pcerts$O \
 $(OBJDIR)$Pchain$O \
 $(OBJDIR)$Pclient$O \
 $(OBJDIR)$Perrors$O \
 $(OBJDIR)$Pfiles$O \
 $(OBJDIR)$Pimpl$O \
 $(OBJDIR)$Pkeys$O \
 $(OBJDIR)$Pnames$O \
 $(OBJDIR)$Pserver$O \
 $(OBJDIR)$Pskey$O \
 $(OBJDIR)$Psslio$O \
 $(OBJDIR)$Pta$O \
 $(OBJDIR)$Ptwrch$O \
 $(OBJDIR)$Pvector$O \
 $(OBJDIR)$Pverify$O \
 $(OBJDIR)$Pxmem$O

all: nc hv hv-cn

hv:
	$(CC) $@.c $(INC) $(CFLAGS) -o hv ${OBJBRSSL} $(BUILD)/libbearssl.a $(LDFLAGS)

nc:
	$(CC) $@.c $(INC) $(CFLAGS) -o nc ${OBJBRSSL} $(BUILD)/libbearssl.a $(LDFLAGS)

hv-cn:
	$(CC) $@.c $(INC) $(CFLAGS) -o hv-cn ${OBJBRSSL} $(BUILD)/libbearssl.a $(LDFLAGS)

.PHONY: clean

clean:
	rm nc hv hv-cn 