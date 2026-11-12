#######
# Grade Analyzer Makefile
#######

.PHONY: clean run

grade-analyzer: main.o grade.o statistics.o
	gcc -o grade-analyzer main.o grade.o statistics.o -lm



main.o: main.c
	gcc -c main.c

grade.o: grade.c
	gcc -c grade.c

statistics.o: statistics.c
	gcc -c statistics.c


run:
	./grade-analyzer sample-grades.csv


clean:
	/bin/rm -f main.o grade.o statistics.o grade-analyzer
