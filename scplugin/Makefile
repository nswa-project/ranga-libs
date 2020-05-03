scplugin.so : main.c
	$(CC) -fPIC -shared  $(CFLAGS) -DRADIUS='""' -DPREFIX1="'\n'" -o $@ $(LDFLAGS) $<

install:
	$(INSTALL_DIR) $(PREFIX)/usr/lib/pppd/2.4.7/
	install -m770 scplugin.so $(PREFIX)/usr/lib/pppd/2.4.7/
