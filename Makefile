OBJ = main.o manager.o system.o resource.o event.o
OPT = -fsanitize=thread -pthread -Wall -Wextra

main: $(OBJ)
	gcc $(OPT) -o main $(OBJ)

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

clean:
	rm -f $(OBJ) main