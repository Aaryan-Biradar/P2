# Object files to be compiled
OBJ = main.o manager.o system.o resource.o event.o

# Compiler options
OPT = -g -Wall -Wextra -pthread

# Target executable
TARGET = p1

# Default target to build the executable
$(TARGET): $(OBJ)
	gcc $(OPT) -o $(TARGET) $(OBJ)

# Rule to compile main.c into main.o
main.o: main.c defs.h
	gcc $(OPT) -c main.c

# Rule to compile manager.c into manager.o
manager.o: manager.c defs.h
	gcc $(OPT) -c manager.c

# Rule to compile system.c into system.o
system.o: system.c defs.h
	gcc $(OPT) -c system.c

# Rule to compile resource.c into resource.o
resource.o: resource.c defs.h
	gcc $(OPT) -c resource.c

# Rule to compile event.c into event.o
event.o: event.c defs.h
	gcc $(OPT) -c event.c

# Clean target to remove all object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)