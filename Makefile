#######
# Grade Analyzer Makefile
#######

CC = gcc
CCFLAGS = -c -O2
LIBS = -lm
RM = /bin/rm

OBJS = main.o grade.o statistics.o


.PHONY: clean run

grade-analyzer: $(OBJS)
	$(CC) -o grade-analyzer $(OBJS) $(LIBS)



main.o: main.c
	$(CC) $(CCFLAGS) main.c

grade.o: grade.c
	$(CC) $(CCFLAGS) grade.c

statistics.o: statistics.c
	$(CC) $(CCFLAGS) statistics.c


run:
	./grade-analyzer sample-grades.csv


clean:
	$(RM) -f main.o grade.o statistics.o grade-analyzer
