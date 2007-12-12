all:dvi pdf

pdf:
	dvipdf bc_pspdftool.dvi
dvi:
	latex bc_pspdftool.tex

ps:dvi
	dvips -D600 -o bc_pspdftool.ps -O-0.5in,-0.5in -t a4 bc_pspdftool.dvi
view:dvi pdf
	open bc_pspdftool.pdf
check:bc_pspdftool.tex
	aspell --lang=cs --mode=tex -c bc_pspdftool.tex 
clean:
	@echo "Cleaning ..."
	@rm *.dvi *.log *.aux *.pdf *.toc *.ps > /dev/null 2>&1 
