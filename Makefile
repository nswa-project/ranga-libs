BINS := http302d su waitrun dispatcher

all : $(BINS)
	$(MAKE) -C nkplugin
	$(MAKE) -C scplugin
	$(MAKE) -C nswaplua
	$(MAKE) -C crypto

http302d : http302d.c
su : su.c
waitrun : waitrun.c
dispatcher : dispatcher.c
http302d su waitrun dispatcher:
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $<

install:
	$(INSTALL_DIR) $(PREFIX)/lib/ranga/libexec
	$(INSTALL_BIN) $(BINS) $(PREFIX)/lib/ranga/libexec
	$(MAKE) -C nkplugin install
	$(MAKE) -C scplugin install
	$(MAKE) -C nswaplua install
	$(MAKE) -C crypto install

.PHONY: all install
