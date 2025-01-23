CC = gcc
CFLAGS = -Wall -g
TARGETS = client server main
LIBS = -pthread

OBJ_CLIENT = client.o
OBJ_SERVER = server.o
OBJ_COURSE = course.o database.o student.o
OBJ_MAIN = main.o course.o database.o student.o

all: $(TARGETS)

client: $(OBJ_CLIENT)
	$(CC) $(CFLAGS) -o $@ $^

server: $(OBJ_SERVER) $(OBJ_COURSE)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

main: $(OBJ_MAIN)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(TARGETS)

.PHONY: all clean
