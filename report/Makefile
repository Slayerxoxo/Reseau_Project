all: main tableofcontents

main: main.tex
	pdflatex main.tex

tableofcontents: main.tex
	pdflatex main.tex

clear:
	rm *.aux *.toc *.log *.out
