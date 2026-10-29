# Student Record Manager (C)

A menu-driven console program for managing student records.

## Features
- Add, display, search, update and delete students
- Sort by GPA or by name
- Statistics (count, average, highest, lowest GPA)
- Saves to and loads from a binary file (`students.dat`)
- Input validation (no crashes on bad input, duplicate IDs rejected)

## C concepts demonstrated
- `struct` and `typedef`
- Dynamic memory (`realloc`, `free`) with a growing array
- Multi-file project with a header file and include guards
- File I/O (`fopen`, `fread`, `fwrite`)
- `qsort` with comparison function pointers
- Safe input using `fgets` and `strtol`/`strtof`
- Makefile build

## Build and run
```
make
./student_manager
```
On Windows without `make`:
```
gcc -Wall -Wextra -std=c11 -o student_manager main.c student.c
.\student_manager
```
