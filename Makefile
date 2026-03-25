#################################################
# Makefile					#
#						#
# Hector Anaya					#
# CSSC2505					#
# CS530, Spring 2026				#
# Assignment #2, SICXE Assembler		#
#################################################

EXEC = asmbl
FILES := $(shell find . -name "*.cpp")
CC = /usr/bin/g++
LFLAGS = -g
CFLAGS = -Wall -g -MMD -MP -I./dataTypes -I./unitTest

OBJECTS = $(FILES:.cpp=.o)
DEPS = $(OBJECTS:.o=.d)

$(EXEC):$(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXEC) $(OBJECTS)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f *.o $(OBJECTS) $(DEPS) $(EXEC)
