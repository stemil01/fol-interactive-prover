CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I. -I$(BUILDDIR)


BUILDDIR = build

# Generated sources
PARSER_CPP = $(BUILDDIR)/parser.cpp
PARSER_HPP = $(BUILDDIR)/parser.hpp
LEXER_CPP  = $(BUILDDIR)/lexer.cpp

# Object files
OBJS = \
	$(BUILDDIR)/main.o \
	$(BUILDDIR)/parser.o \
	$(BUILDDIR)/lexer.o \
	$(BUILDDIR)/fol.o \
	$(BUILDDIR)/itp.o

# Final executable
fol-ip: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $(BUILDDIR)/$@

# Ensure build/ exists
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Compile .cpp -> build/*.o
$(BUILDDIR)/%.o: %.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Bison: generate C++ source + header into build/
$(PARSER_CPP) $(PARSER_HPP): parser.ypp fol.hpp | $(BUILDDIR)
	bison --header=$(PARSER_HPP) -o $(PARSER_CPP) $<

# Flex: generate C++ source into build/
$(LEXER_CPP): lexer.l $(PARSER_HPP) | $(BUILDDIR)
	flex -o $(LEXER_CPP) $<

# Include auto-generated dependency files
-include $(OBJS:.o=.d)

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
