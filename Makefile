BINS=/usr/local/bin/takepix \
     /etc/cron.d/takepixpart \
     /usr/local/bin/datareader \
     /usr/local/bin/datasource \
     /etc/cron.d/datasourcepart \
     /etc/supervisor/conf.d/datareader.conf

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

# Datareader requires "supervisor" to run.
# See apt-get supervisor
/usr/local/bin/datareader: datareader
	sudo cp $^ $@
	sudo chmod 755 $@
	sudo chown root.root $@

/etc/cron.d/datasourcepart: datasourcepart
	sudo cp $^ $@
	sudo chmod 755 $@
	sudo chown root.root $@

/etc/supervisor/conf.d/datareader.conf: datareader.conf
	sudo cp $^ $@
	sudo chmod 755 $@
	sudo chown root.root $@
	sudo supervisorctl reread
	sudo supervisorctl update

clean:
	rm -f *~
