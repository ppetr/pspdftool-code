all:dvi pdf

pdf:
	dvipdf bc_pspdftool_iso8859-2.dvi
dvi:
	latex bc_pspdftool_iso8859-2.tex

ps:dvi
	dvips -D600 -o bc_pspdftool_iso8859-2.ps -O-0.5in,-0.5in -t a4 bc_pspdftool_iso8859-2.dvi
view:dvi pdf
	open bc_pspdftool_iso8859-2.pdf
check:bc_pspdftool_iso8859-2.tex
	aspell --lang=cs --mode=tex -c bc_pspdftool_iso8859-2.tex 
clean:
	@echo "Cleaning ..."
	@rm *.dvi *.log *.aux *.pdf *.toc *.ps > /dev/null 2>&1 
