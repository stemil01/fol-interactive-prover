# Main document name (without .tex)
MAIN = main

# LaTeX engine via latexmk
LATEXMK = latexmk
LATEXMK_FLAGS = -xelatex -interaction=nonstopmode -bibtex -halt-on-error

.PHONY: all pdf clean distclean watch

# Default target
all: pdf

# Build the PDF
pdf:
	$(LATEXMK) $(LATEXMK_FLAGS) $(MAIN)

# Continuous compilation (rebuild on file change)
watch:
	$(LATEXMK) $(LATEXMK_FLAGS) -pvc $(MAIN)

# Remove auxiliary files
clean:
	$(LATEXMK) -c

# Remove everything, including the PDF
distclean:
	$(LATEXMK) -bibtex -C
	rm -f *-SAVE-ERROR
