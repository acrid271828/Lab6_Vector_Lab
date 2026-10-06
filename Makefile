CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES= vector.c vector_storage.c minimat_ui.c main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE= minimat

all: $(SOURCES) $(EXECUTABLE)

# pull in **existing** .o files
-include $(OBJECTS:.o=.d)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $<> $*.d

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) *.d