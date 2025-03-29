OBJ = main.o manager.o system.o resource.o event.o
OPT = -g -Wall -Wextra -pthread
TARGET = p2

# Default target to build the executable
$(TARGET): $(OBJ)
	gcc $(OPT) -o $(TARGET) $(OBJ)

# Rule to compile main.c into main.o
main.o: main.c defs.h
	gcc $(OPT) -c main.c

manager.o: manager.c defs.h
	gcc $(OPT) -c manager.c

system.o: system.c defs.h
	gcc $(OPT) -c system.c

resource.o: resource.c defs.h
	gcc $(OPT) -c resource.c

event.o: event.c defs.h
	gcc $(OPT) -c event.c

# Clean target to remove all object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)