## Makefile für OpenGL
## Autor: Holger Arndt, Sebastian Birk, Martin Galgon
## Datum: 07.12.2015

CPP = cpp
CXX = g++
CFLAGS = -O3 -std=c++11 -Wall -Wextra -pedantic -Wno-unused-parameter
INC = -I/usr/include
GLLIB = -lSDL -lGL -lGLU
DEP = $(CPP) -MM

EXE = $(patsubst %.cc,%,$(wildcard opengl-*.cc))
ALLSRC = $(wildcard *.cc)
ALLDEP = $(patsubst %.cc,%.dep,$(ALLSRC))
ALLOBJ = $(patsubst %.cc,%.o,$(ALLSRC))

.PHONY: default
default: $(EXE)

.SUFFIXES:
.SECONDARY:

$(EXE): $(ALLOBJ)
	$(CXX) -o $@ $^ -L. $(GLLIB)

%.o: %.cc
	$(CXX) -c $(CFLAGS) $(INC) -o $@ $<

%.dep: %.cc
	$(DEP) $< $(INC) | sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@

.PHONY: clean
clean:
	rm -f $(EXE) $(ALLOBJ) $(ALLDEP)

-include $(ALLDEP)
