#######
# Grade Analyzer Makefile
#######


grade-analyzer: grade-analyzer.o
	gcc -o grade-analyzer grade-analyzer.o -lm



grade-analyzer.o: grade-analyzer.c
	gcc -c grade-analyzer.c


clean:
	/bin/rm -f grade-analyzer.o grade-analyzer
