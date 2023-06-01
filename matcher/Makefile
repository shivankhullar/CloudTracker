#
# Makefile for antsontable I/O module
#
# This is a start only; you should edit this Makefile such that it is
# able to run all test using 'make test'.
#
# Type 'make help' for usage.
#

# Variables controlling the settings for the compilation.
# Some of these variables do not need to be set, but are
# included here for completeness.

## read the systype information to use the blocks below for different machines
ifdef SYSTYPE
SYSTYPE := "$(SYSTYPE)"
-include ../Makefile.systype
else
include ../Makefile.systype
endif

ifeq ($(wildcard Makefile.systype), ../Makefile.systype)
INCL = ../Makefile.systype
else
INCL =
endif
FINCL =

#------------------------------------------------------------
ifeq ($(SYSTYPE),"Mac")
CXX = h5c++
#g++
CPPFLAGS =
CXXFLAGS = -std=c++14 -O3 -g -march=native
#CXXFLAGS+= -mmacosx-version-min=13.0
LDFLAGS = -g
LDLIBS = 
MACOSX_DEPLOYMENT_TARGET=13.0
#-lnetcdf_c++4 -lfftw3

export MACOSX_DEPLOYMENT_TARGET := ${MACOSX_DEPLOYMENT_TARGET}

endif
#------------------------------------------------------------


#------------------------------------------------------------
ifeq ($(SYSTYPE),"Niagara")
CXX = h5c++
#g++
CPPFLAGS =
CXXFLAGS = -std=c++14 -O3 -g -march=native
LDFLAGS = -g
LDLIBS = 
#-lnetcdf_c++4 -lfftw3
endif
#------------------------------------------------------------

#------------------------------------------------------------
ifeq ($(SYSTYPE),"CITA")
CXX = h5c++
#g++
CPPFLAGS =
CXXFLAGS = -std=c++14 -O3 -g -march=native
LDFLAGS = -g
LDLIBS = 
#-lnetcdf_c++4 -lfftw3
endif
#------------------------------------------------------------

## all: build everything
all: matcher


## matcher: Run the application 'matcher'
matcher: matcher.o matcher_functions.o structs_and_classes.o
	${CXX} ${LDFLAGS} $^ -o $@ ${LDLIBS}

matcher.o: matcher.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} -c -o $@ $^

matcher_functions.o: matcher_functions.cpp matcher_functions.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} -c -o matcher_functions.o matcher_functions.cpp

structs_and_classes.o: structs_and_classes.cpp structs_and_classes.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} -c -o structs_and_classes.o structs_and_classes.cpp

## run: Run the matcher program
run:
	./matcher > output.dat


## clean: remove intermediate generated files
clean:
	$(RM) matcher output.dat matcher.o matcher_functions.o structs_and_classes.o 

## help: print all commented 'make' targets
# (the sed command displays all lines from the Makefile that start with '##')
help:
	@sed -n 's/^## *\(.*\): */\1:\t/p' Makefile 

# all targets can be files except the following:
.PHONY: all clean help


