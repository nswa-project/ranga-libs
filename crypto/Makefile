ALL := aescrypt2 pk_encrypt pk_verify

all : $(ALL)

%:%.c
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) -lmbedcrypto $<

install:
	$(INSTALL_DIR) $(PREFIX)/usr/bin
	$(INSTALL_BIN) aescrypt2 $(PREFIX)/usr/bin
	$(INSTALL_BIN) pk_encrypt $(PREFIX)/usr/bin
	$(INSTALL_BIN) pk_verify $(PREFIX)/usr/bin
