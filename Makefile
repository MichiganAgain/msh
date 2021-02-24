objects = main.o

main: $(objects)
	gcc --std=c18 -o main $(objects)

