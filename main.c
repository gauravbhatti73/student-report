/*
 * main.c - Student Record Manager
 * Menu-driven program to add, view, search, update, delete, sort,
 * and persist student records.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "student.h"

/* ---------- Input helpers (safe against bad input) ---------- */

static void read_line(const char *prompt, char *buf, size_t len) {
    printf("%s", prompt);
    if (fgets(buf, (int)len, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    buf[strcspn(buf, "\n")] = '\0';
}

static int read_int(const char *prompt) {
    char buf[64];
    char *end;
    for (;;) {
        read_line(prompt, buf, sizeof(buf));
        long v = strtol(buf, &end, 10);
        if (end != buf && *end == '\0') {
            return (int)v;
        }
        printf("Invalid number, try again.\n");
    }
}

static float read_gpa(const char *prompt) {
    char buf[64];
    char *end;
    for (;;) {
        read_line(prompt, buf, sizeof(buf));
        float v = strtof(buf, &end);
        if (end != buf && *end == '\0' && v >= 0.0f && v <= 4.0f) {
            return v;
        }
        printf("GPA must be a number between 0.0 and 4.0.\n");
    }
}

static void read_name(const char *prompt, char *name) {
    for (;;) {
        read_line(prompt, name, NAME_LEN);
        if (name[0] != '\0') {
            return;
        }
        printf("Name cannot be empty.\n");
    }
}

/* ---------- Menu actions ---------- */

static void add_student(StudentList *list) {
    Student s;
    s.id = read_int("Enter ID: ");
    if (list_find(list, s.id) != -1) {
        printf("A student with ID %d already exists.\n", s.id);
        return;
    }
    read_name("Enter name: ", s.name);
    s.gpa = read_gpa("Enter GPA (0.0 - 4.0): ");
    if (list_add(list, s) == 0) {
        printf("Student added.\n");
    } else {
        printf("Could not add student.\n");
    }
}

static void search_student(const StudentList *list) {
    int id = read_int("Enter ID to search: ");
    int idx = list_find(list, id);
    if (idx == -1) {
        printf("No student with ID %d.\n", id);
        return;
    }
    printf("%-8s %-30s %s\n", "ID", "Name", "GPA");
    student_print(&list->items[idx]);
}

static void update_student(StudentList *list) {
    int id = read_int("Enter ID to update: ");
    int idx = list_find(list, id);
    if (idx == -1) {
        printf("No student with ID %d.\n", id);
        return;
    }
    read_name("New name: ", list->items[idx].name);
    list->items[idx].gpa = read_gpa("New GPA (0.0 - 4.0): ");
    printf("Student updated.\n");
}

static void delete_student(StudentList *list) {
    int id = read_int("Enter ID to delete: ");
    if (list_remove(list, id) == 0) {
        printf("Student deleted.\n");
    } else {
        printf("No student with ID %d.\n", id);
    }
}

static void show_statistics(const StudentList *list) {
    if (list->size == 0) {
        printf("No data available.\n");
        return;
    }
    float sum = 0.0f;
    const Student *best = &list->items[0];
    const Student *worst = &list->items[0];
    for (size_t i = 0; i < list->size; i++) {
        sum += list->items[i].gpa;
        if (list->items[i].gpa > best->gpa) best = &list->items[i];
        if (list->items[i].gpa < worst->gpa) worst = &list->items[i];
    }
    printf("Students: %zu\n", list->size);
    printf("Average GPA: %.2f\n", sum / (float)list->size);
    printf("Highest: %s (%.2f)\n", best->name, best->gpa);
    printf("Lowest:  %s (%.2f)\n", worst->name, worst->gpa);
}

static void print_menu(void) {
    printf("\n===== Student Record Manager =====\n");
    printf(" 1. Add student\n");
    printf(" 2. Display all students\n");
    printf(" 3. Search by ID\n");
    printf(" 4. Update student\n");
    printf(" 5. Delete student\n");
    printf(" 6. Sort by GPA (highest first)\n");
    printf(" 7. Sort by name (A-Z)\n");
    printf(" 8. Show statistics\n");
    printf(" 9. Save to file\n");
    printf(" 0. Exit\n");
}

int main(void) {
    StudentList list;
    list_init(&list);

    if (list_load(&list, DATA_FILE) == 0) {
        printf("Loaded %zu record(s) from %s.\n", list.size, DATA_FILE);
    } else {
        list_free(&list); /* discard any partial load */
        printf("No saved data found. Starting fresh.\n");
    }

    int choice;
    do {
        print_menu();
        choice = read_int("Choose an option: ");
        switch (choice) {
            case 1: add_student(&list); break;
            case 2: list_print(&list); break;
            case 3: search_student(&list); break;
            case 4: update_student(&list); break;
            case 5: delete_student(&list); break;
            case 6: list_sort_by_gpa(&list); list_print(&list); break;
            case 7: list_sort_by_name(&list); list_print(&list); break;
            case 8: show_statistics(&list); break;
            case 9:
                if (list_save(&list, DATA_FILE) == 0) {
                    printf("Saved %zu record(s).\n", list.size);
                } else {
                    printf("Error saving file.\n");
                }
                break;
            case 0:
                if (list_save(&list, DATA_FILE) == 0) {
                    printf("Data saved. Goodbye!\n");
                } else {
                    printf("Warning: could not save data. Goodbye!\n");
                }
                break;
            default: printf("Invalid option.\n");
        }
    } while (choice != 0);

    list_free(&list);
    return 0;
}
