# Sam Lacet-Brown
# Makefile for Six Degrees
#
# Project 2 (fall 2020)
# 

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -std=c++11 
INCLUDES = $(shell echo *.h)

SixDegrees: main.o SixDegrees.o CollabGraph.o Artist.o
	${CXX} -o $@ $^

%.o: %.cpp ${INCLUDES}
	${CXX} ${CXXFLAGS} -c $<

clean:
	rm -rf SixDegrees *.o *.dSYM

providephase1:
	provide comp15 proj2phase1 SixDegrees.cpp SixDegrees.h \
			main.cpp README Makefile testartists.txt \
			testcommands.txt testoutput.txt

providephase2:
	provide comp15 proj2phase2 SixDegrees.cpp SixDegrees.h \
			main.cpp README Makefile TestingFunctions.cpp \
			testartists.txt testcommands.txt testcommands \
			testoutput.txt testoutput2.txt
			
