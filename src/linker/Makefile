#
# Makefile for linker module
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

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++14 -O3 -g -march=native
HDF5_INC = /usr/local/Cellar/hdf5/1.14.1/include
HDF5_LIB = 
#/usr/local/Cellar/hdf5/1.14.1/lib
LDFLAGS = 
#-g
LDLIBS = -lhdf5 -lz
MACOSX_DEPLOYMENT_TARGET=14.0
#-lnetcdf_c++4 -lfftw3

export MACOSX_DEPLOYMENT_TARGET := ${MACOSX_DEPLOYMENT_TARGET}

endif
#------------------------------------------------------------


#------------------------------------------------------------
ifeq ($(SYSTYPE),"Niagara")

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++14 -O3 -g -march=native
HDF5_INC = ${SCINET_HDF5_ROOT}/include
#need to include -L here itself
HDF5_LIB = -L${SCINET_HDF5_ROOT}/lib

LDFLAGS = -g
LDLIBS = -lhdf5 -lz
endif
#------------------------------------------------------------

#------------------------------------------------------------
ifeq ($(SYSTYPE),"CITA")

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++14 -O3 -g -march=native
HDF5_INC = 
#${SCINET_HDF5_ROOT}/include
HDF5_LIB = -L
#${SCINET_HDF5_ROOT}/lib

LDFLAGS = -g
LDLIBS = -lhdf5 -lz
endif
#------------------------------------------------------------

#-lnetcdf_c++4 -lfftw3

## all: build everything
all: linker

## gwaves: Run the linker application
linker: linker.o linker_functions.o structs_and_classes.o utilities.o read_params.o io_hdf5.o io_txt.o
	${CXX} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} $^ -o $@ ${LDLIBS}

linker.o: linker.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o $@ $^

linker_functions.o: linker_functions.cpp linker_functions.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o linker_functions.o linker_functions.cpp

structs_and_classes.o: structs_and_classes.cpp structs_and_classes.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o structs_and_classes.o structs_and_classes.cpp

read_params.o: read_params.cpp read_params.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o read_params.o read_params.cpp

utilities.o: utilities.cpp utilities.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o utilities.o utilities.cpp

io_hdf5.o: io_hdf5.cpp io.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o io_hdf5.o io_hdf5.cpp

io_txt.o: io_txt.cpp io.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} ${HDF5_LIB} ${LDFLAGS} -c -o io_txt.o io_txt.cpp
	
## run: Run the linker program
run:
	./linker > output.dat


## clean: remove intermediate generated files
clean:
	$(RM) linker output.dat linker.o linker_functions.o read_params.o structs_and_classes.o utilities.o io_hdf5.o io_txt.o

## help: print all commented 'make' targets
# (the sed command displays all lines from the Makefile that start with '##')
help:
	@sed -n 's/^## *\(.*\): */\1:\t/p' Makefile 

# all targets can be files except the following:
.PHONY: all clean help


