CCOMMAND = gcc
CFLAGS = -Wall -c -Wextra
SOURCES = $(wildcard src/*.c)
OBJECTS = $(SOURCES:.c=.o)
INC_DIRS = -I./inc
EXE_NAME = vm_exec


$(EXE_NAME): $(OBJECTS)
	$(CCOMMAND) $(OBJECTS) -o $(EXE_NAME)

%.o: %.c
	$(CCOMMAND) $(INC_DIRS) -c $(CFLAGS) $< -o $@


clean:
	rm -f $(EXE_NAME) $(OBJECTS)
	
