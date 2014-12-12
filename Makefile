
CC=gcc
CPPC=g++
CFLAGS= -Wall -lm
DEBUGFLAGS= -g -O0 -fno-inline
OPTFLAGS= -O2

c: clean
c: COMPILER=$(CC)
c: CFLAGS += $(OPTFLAGS)
c: tabu

c-debug: clean
c-debug: COMPILER=$(CC)
c-debug: CFLAGS += $(DEBUGFLAGS)
c-debug: tabu

cpp: clean
cpp: COMPILER=$(CPPC)
cpp: CFLAGS += $(OPTFLAGS)
cpp: tabu

cpp-debug: clean
cpp-debug: COMPILER=$(CPPC)
cpp-debug: CFLAGS += $(DEBUGFLAGS)
cpp-debug: tabu

tabu: main.o tabu.o
	$(COMPILER) $(CFLAGS) -o tabu main.o tabu.o
	
main.o:
	$(COMPILER) $(CFLAGS) -o main.o -c main.cpp 

tabu.o: 
	$(COMPILER) $(CFLAGS) -o tabu.o -c tabu.cpp 

clean: 
	rm -rf *.o

