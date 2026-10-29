/*
 * student.c - Implementation of the student list (dynamic array + file I/O).
 */
#include "student.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 4

void list_init(StudentList *list) {
    list->items = NULL;
    list->size = 0;
    list->capacity = 0;
}

void list_free(StudentList *list) {
    free(list->items);
    list_init(list);
}

int list_find(const StudentList *list, int id) {
    for (size_t i = 0; i < list->size; i++) {
        if (list->items[i].id == id) {
            return (int)i;
        }
    }
    return -1;
}

int list_add(StudentList *list, Student s) {
    if (list_find(list, s.id) != -1) {
        return -1; /* duplicate ID */
    }
    if (list->size == list->capacity) {
        size_t new_cap = list->capacity ? list->capacity * 2 : INITIAL_CAPACITY;
        Student *tmp = realloc(list->items, new_cap * sizeof(Student));
        if (tmp == NULL) {
            return -1;
        }
        list->items = tmp;
        list->capacity = new_cap;
    }
    list->items[list->size++] = s;
    return 0;
}

int list_remove(StudentList *list, int id) {
    int idx = list_find(list, id);
    if (idx == -1) {
        return -1;
    }
    /* Shift everything after idx one slot to the left. */
    memmove(&list->items[idx], &list->items[idx + 1],
            (list->size - (size_t)idx - 1) * sizeof(Student));
    list->size--;
    return 0;
}

static int cmp_gpa_desc(const void *a, const void *b) {
    float x = ((const Student *)a)->gpa;
    float y = ((const Student *)b)->gpa;
    return (x < y) - (x > y);
}

static int cmp_name(const void *a, const void *b) {
    return strcmp(((const Student *)a)->name, ((const Student *)b)->name);
}

void list_sort_by_gpa(StudentList *list) {
    qsort(list->items, list->size, sizeof(Student), cmp_gpa_desc);
}

void list_sort_by_name(StudentList *list) {
    qsort(list->items, list->size, sizeof(Student), cmp_name);
}

int list_save(const StudentList *list, const char *path) {
    FILE *fp = fopen(path, "wb");
    if (fp == NULL) {
        return -1;
    }
    if (fwrite(&list->size, sizeof(size_t), 1, fp) != 1 ||
        (list->size > 0 &&
         fwrite(list->items, sizeof(Student), list->size, fp) != list->size)) {
        fclose(fp);
        return -1;
    }
    return fclose(fp) == 0 ? 0 : -1;
}

int list_load(StudentList *list, const char *path) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return -1;
    }
    size_t count;
    if (fread(&count, sizeof(size_t), 1, fp) != 1) {
        fclose(fp);
        return -1;
    }
    for (size_t i = 0; i < count; i++) {
        Student s;
        if (fread(&s, sizeof(Student), 1, fp) != 1 || list_add(list, s) != 0) {
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);
    return 0;
}

void student_print(const Student *s) {
    printf("%-8d %-30s %.2f\n", s->id, s->name, s->gpa);
}

void list_print(const StudentList *list) {
    if (list->size == 0) {
        printf("No students to display.\n");
        return;
    }
    printf("%-8s %-30s %s\n", "ID", "Name", "GPA");
    printf("-------------------------------------------\n");
    for (size_t i = 0; i < list->size; i++) {
        student_print(&list->items[i]);
    }
}
