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

CXX = h5c++
#g++
CPPFLAGS =
CXXFLAGS = -std=c++14 -O3 -g -march=native
LDFLAGS = -g
LDLIBS = 
#-lnetcdf_c++4 -lfftw3

## all: build everything
all: matcher

## gwaves: Run the application 'gwaves'
matcher: matcher.o matcher_functions.o structs_and_classes.o
	${CXX} ${LDFLAGS} $^ -o $@ ${LDLIBS}

matcher.o: matcher.cpp
	${CXX} ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} -c -o $@ $^

matcher_functions.o: matcher_functions.cpp matcher_functions.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} -c -o matcher_functions.o matcher_functions.cpp

structs_and_classes.o: structs_and_classes.cpp structs_and_classes.h
	${CXX} ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS} -c -o structs_and_classes.o structs_and_classes.cpp

## run: Run the GW Match Filtered Algorithm program
run:
	./matcher > output.dat

## result: Generate the results from a python script.
result: generate_results.py
	python generate_results.py

## clean: remove intermediate generated files
clean:
	$(RM) matcher output.dat matcher.o matcher_functions.o structs_and_classes.o 

## help: print all commented 'make' targets
# (the sed command displays all lines from the Makefile that start with '##')
help:
	@sed -n 's/^## *\(.*\): */\1:\t/p' Makefile 

# all targets can be files except the following:
.PHONY: all clean help


