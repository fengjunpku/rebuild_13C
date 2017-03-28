###############################################################################

#include $(ROOTSYS)/etc/Makefile.arch

###############################################################################
OBJ = rebuild_0.exe
MainFile = re0_main2.cc
LinkFile = LinkDef.hh
UserDict = JunDict.cc
###############################################################################

SourceFile := $(wildcard src/*.cc) 
IncludeFile := $(wildcard include/*.hh) 

###############################################################################

ROOTCFLAGS  = $(shell root-config --cflags)
#ROOTLIBS    = $(shell root-config --libs)
ROOTGLIBS = $(shell root-config --glibs)

GXX = g++ -std=c++0x
DIR_INC = -Iinclude
MoreWarn = -Wshadow -Wcast-qual 
#-Wunreachable-code
#-I$(TARTSYS)/include
CFLAGS = -Wall -Wextra -O2 $(DIR_INC) -lTMVA -lMathMore -lSpectrum $(MoreWarn)
#-fopenmp 
#-L$(TARTSYS)/lib
#-lXMLParser -lanacore -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
###############################################################################

all:$(OBJ)
$(OBJ): $(MainFile) $(SourceFile) $(UserDict)
	$(GXX) $(CFLAGS) $(ROOTCFLAGS) $(ROOTGLIBS) -o $@ $(MainFile) $(SourceFile) $(UserDict)
	@echo "================================"
	@echo "Compile $@ done !"
	@echo "================================"

$(UserDict): $(IncludeFile) $(LinkFile)
	@echo "================================"
	@echo " Generating dictionary $@ ..."
	@echo "================================"
	rootcint -f $@ -c $(DIR_INC) $^

clean:
	rm -f *.o *.d *Dict.* *.root *.txt $(OBJ)
	
###############################################################################

