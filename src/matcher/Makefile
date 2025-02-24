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
#CXX = h5c++
CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++14 -O3 -g -march=native
HDF5_INC = /usr/local/Cellar/hdf5/1.14.3/include
HDF5_LIB = 
#CXXFLAGS+= -mmacosx-version-min=14.0
LDFLAGS = -g
LDLIBS = -lhdf5 -lz
MACOSX_DEPLOYMENT_TARGET=14.0
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
#CXX = h5c++
CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++14 -O3 -g -march=native
HDF5_INC =  
HDF5_LIB = -L
LDFLAGS = -g
LDLIBS = -lhdf5 -lz
#-lnetcdf_c++4 -lfftw3
endif
#------------------------------------------------------------

## all: build everything
all: matcher


## matcher: Run the application 'matcher'
matcher: ./main/matcher.o ./main/matcher_functions.o ./utils/structs_and_classes.o ./utils/utilities.o ./io/read_params.o ./io/io.o 
	${CXX} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} $^ -o $@ ${LDLIBS}

matcher.o: ./main/matcher.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o $@ $^

matcher_functions.o: ./main/matcher_functions.cpp ../../include/matcher/matcher_functions.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o matcher_functions.o matcher_functions.cpp

structs_and_classes.o: ./utils/structs_and_classes.cpp ../../include/matcher/structs_and_classes.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o structs_and_classes.o structs_and_classes.cpp

read_params.o: ./io/read_params.cpp ../../include/matcher/read_params.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o read_params.o read_params.cpp

io.o: ./io/io.cpp ../../include/matcher/io.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o io.o io.cpp

utilities.o: ./utils/utilities.cpp ../../include/matcher/utilities.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o utilities.o utilities.cpp

## run: Run the matcher program
run:
	./matcher > output.dat


## clean: remove intermediate generated files
clean:
	$(RM) matcher output.dat ./main/matcher.o ./main/matcher_functions.o ./utils/structs_and_classes.o ./utils/utilities.o ./io/read_params.o ./io/io.o 

## help: print all commented 'make' targets
# (the sed command displays all lines from the Makefile that start with '##')
help:
	@sed -n 's/^## *\(.*\): */\1:\t/p' Makefile 

# all targets can be files except the following:
.PHONY: all clean help


