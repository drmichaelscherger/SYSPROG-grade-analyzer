# Grade Analyzer refactoring starter

This directory contains the working, single-file program for the modular C
refactoring lab.  Before changing the program, compile and run the baseline:

```sh
gcc -std=c17 -Wall -Wextra -Wpedantic -g grade-analyzer.c \
   -o grade-analyzer -lm
./grade-analyzer sample-grades.csv
```

Save its output so you can compare it with the modular version later:

```sh
./grade-analyzer sample-grades.csv > expected-output.txt
```

Separate the program into `main`, `grades`, and `statistics` modules and create its Makefile.  
