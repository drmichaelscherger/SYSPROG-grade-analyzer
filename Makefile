#######
# Grade Analyzer Makefile
#######

CC = gcc
CCFLAGS = -c -O2
LIBS = -lm
RM = /bin/rm

OBJS = main.o grade.o statistics.o

# $<   : use the name of the current file
# $@   : use the name of the target
# $^   : use the name(s) of the dependencies




.PHONY: clean run

grade-analyzer: $(OBJS)
	$(CC) -o $@ $^ $(LIBS)



main.o: main.c
	$(CC) $(CCFLAGS) $<

grade.o: grade.c
	$(CC) $(CCFLAGS) $<

statistics.o: statistics.c
	$(CC) $(CCFLAGS) $<


run:
	./grade-analyzer sample-grades.csv


clean:
	$(RM) -f main.o grade.o statistics.o grade-analyzer
