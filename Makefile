#################################################
# Makefile					#
#						#
# Hector Anaya					#
# CSSC2505					#
# CS530, Spring 2026				#
# Assignment #2, SICXE Assembler		#
#################################################

EXEC = asmbl
FILES = asmbl.cpp
CC = /usr/bin/g++
LFLAGS = -g
CFLAGS = -Wall -g 

OBJECTS = $(FILES:.cpp=.o)

$(EXEC):$(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXEC) $(OBJECTS)
	rm -f *.o

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXEC)
