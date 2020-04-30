VER_MAJOR := 0
VER_MINOR := 1.0

SO := libnswa.so
SONAME := $(SO).$(VER_MAJOR)
OUT := $(SO).$(VER_MAJOR).$(VER_MINOR)

OBJS := main.o

all : $(SO)

$(SO) : $(OBJS)
	$(CC) $(LDFLAGS) -shared --pic -Wl,-soname,$(SONAME) -llua -o $@ $(OBJS)

%.o : %.c
	$(CC) -fPIC $(CFLAGS) -o $@ -c $<

install:
	$(INSTALL_DIR) $(PREFIX)/usr/lib/lua/
	$(INSTALL_BIN) libnswa.so $(PREFIX)/usr/lib/lua/
