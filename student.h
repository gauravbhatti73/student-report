/*
 * student.h - Student record data structures and list operations.
 */
#ifndef STUDENT_H
#define STUDENT_H

#include <stddef.h>

#define NAME_LEN 50
#define DATA_FILE "students.dat"

typedef struct {
    int id;
    char name[NAME_LEN];
    float gpa;
} Student;

/* A dynamically growing array of students. */
typedef struct {
    Student *items;
    size_t size;
    size_t capacity;
} StudentList;

void list_init(StudentList *list);
void list_free(StudentList *list);

/* Returns 0 on success, -1 on failure (out of memory or duplicate ID). */
int list_add(StudentList *list, Student s);

/* Returns the index of the student with this ID, or -1 if not found. */
int list_find(const StudentList *list, int id);

/* Returns 0 on success, -1 if the ID does not exist. */
int list_remove(StudentList *list, int id);

void list_sort_by_gpa(StudentList *list);
void list_sort_by_name(StudentList *list);

/* Returns 0 on success, -1 on error. */
int list_save(const StudentList *list, const char *path);
int list_load(StudentList *list, const char *path);

void student_print(const Student *s);
void list_print(const StudentList *list);

#endif
