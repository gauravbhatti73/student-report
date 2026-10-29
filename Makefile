CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = student_manager
OBJS = main.o student.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c student.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET) students.dat

.PHONY: all clean
