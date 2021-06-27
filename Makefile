# Makefile for KTH's code.

include Makefile.inc

SOURCES = src/*.cpp src/extra/*.cpp src/extra/coldet/*.cpp 

OBJECTS = $(patsubst %.cpp, %.o, $(wildcard $(SOURCES)))
DEPENDS = $(patsubst %.cpp, %.d, $(wildcard $(SOURCES)))

SDL_LIB = -lSDL2 
GLUT_LIB = -framework OpenGL -framework GLUT -framework cocoa
BASS_LIB = -l bass

LIBS = $(SDL_LIB) $(GLUT_LIB) $(BASS_LIB)

all:	main

main:	$(DEPENDS) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(LIBS) -o $@

%.d: %.cpp
	@$(CXX) -M -MT "$*.o $@" $(CXXFLAGS) $<  > $@
	@echo Generating new dependencies for $<

run:
	./main

clean:
	rm -f $(OBJECTS) $(DEPENDS) main *.pyc

-include $(SOURCES:.cpp=.d)

