CC=gcc
CFLAGS=-c -Wall
LDFLAGS= -lrt

SOURCES_PATH=src/

SOURCES=src/main.c\
src/timer_linux.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=timer_linux

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(SOURCES_PATH)*.o $(EXECUTABLE)
