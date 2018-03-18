all:
	-rm output/ -r -f
	doxygen
#generate PDF e.g by MiKTe
	make -C output/latex
	cp -f output/latex/refman.pdf ../lib/jaglib.pdf