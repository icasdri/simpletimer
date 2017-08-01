.PHONY: clean

simpletimer: simpletimer.pro *.cpp *.h *.ui
	qmake -o Makefile.qmake $<
	make -f Makefile.qmake

clean:
	rm simpletimer
	rm Makefile.qmake
	rm ui_*.h
	rm moc_*
