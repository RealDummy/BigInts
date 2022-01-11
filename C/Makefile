#------------------------------------------------------------------------------
#  Makefile for CSE 101 Programming Assignment 7
#
#  make                     makes Arithmetic
#  make clean               removes all binaries
#  make memcheck            tests for memory leaks
#------------------------------------------------------------------------------

CLIENT         = Arithmetic
ADT1           = BigInteger
ADT2           = List
SOURCE         = $(CLIENT).c
OBJECT         = $(CLIENT).o
ADT1_SOURCE    = $(ADT1).c
ADT1_OBJECT    = $(ADT1).o
ADT1_HEADER    = $(ADT1).h
ADT2_SOURCE    = $(ADT2).c
ADT2_OBJECT    = $(ADT2).o
ADT2_HEADER    = $(ADT2).h
TESTER1        = $(ADT1)Test
TESTER1_OBJ    = $(TESTER1).o
TESTER1_SRC    = $(TESTER1).c
TESTER2        = $(ADT2)Test
TESTER2_OBJ    = $(TESTER2).o
TESTER2_SRC    = $(TESTER2).c
COMPILE        = gcc -c -std=c11 -Wall -Wextra
LINK           = gcc -o
REMOVE         = rm -f
MEMCHECK       = valgrind --leak-check=full

$(CLIENT) : $(OBJECT) $(ADT1_OBJECT) $(ADT2_OBJECT)
	$(LINK) $(CLIENT) $(OBJECT) $(ADT1_OBJECT) $(ADT2_OBJECT)

$(OBJECT) : $(SOURCE) $(ADT1_HEADER) $(ADT2_HEADER)
	$(COMPILE) $(SOURCE)

$(ADT1_OBJECT) : $(ADT1_SOURCE) $(ADT1_HEADER)
	$(COMPILE) $(ADT1_SOURCE)

$(ADT2_OBJECT) : $(ADT2_SOURCE) $(ADT2_HEADER)
	$(COMPILE) $(ADT2_SOURCE)

$(TESTER1) : $(TESTER1_OBJ) $(ADT1_OBJECT) $(ADT2_OBJECT)
	$(LINK) $(TESTER1) $(TESTER1_OBJ) $(ADT1_OBJECT) $(ADT2_OBJECT)

$(TESTER1_OBJ) : $(TESTER1_SRC) $(ADT1_HEADER) $(ADT2_HEADER)
	$(COMPILE) $(TESTER1_SRC)

$(TESTER2) : $(TESTER2_OBJ) $(ADT2_OBJECT)
	$(LINK) $(TESTER2) $(TESTER2_OBJ) $(ADT2_OBJECT)

$(TESTER2_OBJ) : $(TESTER2_SRC) $(ADT2_HEADER)
	$(COMPILE) $(TESTER2_SRC)

clean :
	$(REMOVE) $(CLIENT) $(OBJECT) $(ADT1_OBJECT) $(ADT2_OBJECT) $(TESTER1) $(TESTER1_OBJ) $(TESTER2) $(TESTER2_OBJ)

memcheck : $(CLIENT)
	$(MEMCHECK) $(CLIENT) in3 myout3