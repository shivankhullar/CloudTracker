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

CXX = g++
CPPFLAGS =
CXXFLAGS = -std=c++14 -O3 -g -march=native
HDF5_INC = ${SCINET_HDF5_ROOT}/include
HDF5_LIB = ${SCINET_HDF5_ROOT}/lib

LDFLAGS = -g
LDLIBS = -lhdf5 -lz
#-lnetcdf_c++4 -lfftw3

## all: build everything
all: linker

## gwaves: Run the application 'gwaves'
linker: linker.o linker_functions.o linker_structs.o
	${CXX} -I${HDF5_INC} -L${HDF5_LIB} ${LDFLAGS} $^ -o $@ ${LDLIBS}

linker.o: linker.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} -L${HDF5_LIB} ${LDFLAGS} -c -o $@ $^

linker_functions.o: linker_functions.cpp linker_functions.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} -L${HDF5_LIB} ${LDFLAGS} -c -o linker_functions.o linker_functions.cpp

linker_structs.o: linker_structs.cpp linker_structs.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} -I${HDF5_INC} -L${HDF5_LIB} ${LDFLAGS} -c -o linker_structs.o linker_structs.cpp

## run: Run the GW Match Filtered Algorithm program
run:
	./linker > output.dat

## result: Generate the results from a python script.
result: generate_results.py
	python generate_results.py

## clean: remove intermediate generated files
clean:
	$(RM) linker output.dat linker.o linker_functions.o linker_structs.o 

## help: print all commented 'make' targets
# (the sed command displays all lines from the Makefile that start with '##')
help:
	@sed -n 's/^## *\(.*\): */\1:\t/p' Makefile 

# all targets can be files except the following:
.PHONY: all clean help


