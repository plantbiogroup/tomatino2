BINS=/usr/local/bin/takepix \
     /etc/cron.d/takepixpart \
     /usr/local/bin/datasource \
     /etc/cron.d/datasourcepart

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

/usr/local/bin/datasource: datasource
	sudo cp $^ $@
	sudo chmod 755 $@
	sudo chown root.root $@

/etc/cron.d/datasourcepart: datasourcepart
	sudo cp $^ $@
	sudo chmod 755 $@
	sudo chown root.root $@

clean:
	rm -f *~
