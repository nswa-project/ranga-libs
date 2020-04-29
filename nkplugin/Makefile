all : nkplugin.so seth-ranga

libseth:
	$(MAKE) -C libseth CFLAGS+="-fPIC" libseth-static.a

nkplugin.so : main.c libseth
	$(CC) -fPIC -shared  $(CFLAGS) -Ilibseth -o $@ $(LDFLAGS) $< libseth/libseth-static.a

seth-ranga : seth-ranga.c libseth
	$(CC) -no-pie -fno-pie $(CFLAGS) -Ilibseth -o $@ $(LDFLAGS) $< libseth/libseth-static.a

install:
	$(INSTALL_DIR) $(PREFIX)/usr/lib/pppd/2.4.7/
	$(INSTALL_DIR) $(PREFIX)/usr/bin/
	install -m770 nkplugin.so $(PREFIX)/usr/lib/pppd/2.4.7/
	install -m755 seth-ranga $(PREFIX)/usr/bin/

.PHONY: libseth
