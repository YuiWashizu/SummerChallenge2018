PROJECT=20180702_SummerChallenge2018
TEX_FILE=$(PROJECT).tex
DVI_FILE=$(PROJECT).dvi
PS_FILE=$(PROJECT).ps
PDF_FILE=$(PROJECT).pdf

LATEX = uplatex
PDFTEX = pdflatex
PS2PDF = ps2pdf
DVIPS = pdvips
DVIPDF = dvipdfmx

all: $(PDF_FILE)

$(PDF_FILE): $(DVI_FILE)
#	$(PS2PDF) $<
	$(DVIPDF) $<

$(PS_FILE): $(DVI_FILE)
	$(DVIPS) $< -o $@

$(DVI_FILE): $(TEX_FILE)
	$(LATEX) $<
	$(LATEX) $<
	$(LATEX) $<

.PHONY: show clean all

show: $(PDF_FILE)
	@acroread $(PDF_FILE) &

clean:
	@rm -f *~ *.aux *.toc *.dvi *.pdf *.ps *.log

