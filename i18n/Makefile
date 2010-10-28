.PHONY: clean i18n

i18n: de.mo

%.mo: %.po
	msgfmt -co $*.mo $<

clean:
	-rm *.mo
