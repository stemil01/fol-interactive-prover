CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I. -I$(BUILDDIR)

BUILDDIR = build
SRCDIR = src
PARSERDIR = parsers

# Generated sources
FORMULA_PARSER_CPP = $(BUILDDIR)/formula_parser.cpp
FORMULA_PARSER_HPP = $(BUILDDIR)/formula_parser.hpp
FORMULA_LEXER_CPP  = $(BUILDDIR)/formula_lexer.cpp

TERM_PARSER_CPP = $(BUILDDIR)/term_parser.cpp
TERM_PARSER_HPP = $(BUILDDIR)/term_parser.hpp
TERM_LEXER_CPP  = $(BUILDDIR)/term_lexer.cpp

# Object files
OBJS = \
	$(BUILDDIR)/main.o \
	$(BUILDDIR)/formula_parser.o \
	$(BUILDDIR)/term_parser.o \
	$(BUILDDIR)/formula_lexer.o \
	$(BUILDDIR)/term_lexer.o \
	$(BUILDDIR)/fol.o \
	$(BUILDDIR)/itp.o \
	$(BUILDDIR)/nd.o \

# Final executable
folip: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(BUILDDIR)/$@

# Ensure build/ exists
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compile .cpp -> build/*.o
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Bison: generate C++ source + header into build/
$(FORMULA_PARSER_CPP) $(FORMULA_PARSER_HPP): $(PARSERDIR)/formula_parser.ypp $(SRCDIR)/fol.hpp | $(BUILDDIR)
	bison --header=$(FORMULA_PARSER_HPP) -o $(FORMULA_PARSER_CPP) $<

$(TERM_PARSER_CPP) $(TERM_PARSER_HPP): $(PARSERDIR)/term_parser.ypp $(SRCDIR)/fol.hpp | $(BUILDDIR)
	bison --header=$(TERM_PARSER_HPP) -o $(TERM_PARSER_CPP) $<

# Flex: generate C++ source into build/
$(FORMULA_LEXER_CPP): $(PARSERDIR)/formula_lexer.l $(FORMULA_PARSER_HPP) | $(BUILDDIR)
	flex -o $(FORMULA_LEXER_CPP) $<

$(TERM_LEXER_CPP): $(PARSERDIR)/term_lexer.l $(TERM_PARSER_HPP) | $(BUILDDIR)
	flex -o $(TERM_LEXER_CPP) $<

# Include auto-generated dependency files
-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
