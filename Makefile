objects = main.o

main: $(objects)
	gcc --std=c18 -g -o main $(objects)

main.o: main.c
	gcc --std=c18 -g -c -o main.o main.c

