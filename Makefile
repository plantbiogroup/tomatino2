BINS=/usr/local/bin/takepix \
     /usr/local/bin/takepixpart

install: $(BINS)

uninstall:
	rm -f $(BINS)


/usr/local/bin/takepix: takepix
	sudo cp $^ $@
	sudo chmod 755 $@
	sudo chown root.root $@

/etc/cron.d/takepixpart: takepixpart
	sudo cp $^ $@
	sudo chmod 755 $@
	sudo chown root.root $@

clean:
	rm -f *~
