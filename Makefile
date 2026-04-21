#################################################
# Makefile					#
#						#
# Hector Anaya					#
# CSSC2505					#
# CS530, Spring 2026				#
# Assignment #2, SICXE Assembler		#
#################################################

EXEC = lxe
FILES := $(shell find . -name "*.cpp")
CC = /usr/bin/g++
LFLAGS = -g -no-pie #avoids randomization of the executable
CFLAGS = -std=c++17 -Wall -g -MMD -MP -I./dataTypes -I./unitTest -I./assembler

OBJECTS = $(FILES:.cpp=.o)
DEPS = $(OBJECTS:.o=.d)

$(EXEC):$(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXEC) $(OBJECTS)

%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	rm -f *.o $(OBJECTS) $(DEPS) $(EXEC)
