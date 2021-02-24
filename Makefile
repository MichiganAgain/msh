objects = main.o msh.o signalHandlers.o
objectCompilerFlags = -g -c -o

main: $(objects)
	gcc --std=c18 -g -o main $(objects)

main.o: main.c
	gcc --std=c18 $(objectCompilerFlags) main.o main.c

msh.o: msh.c
	gcc --std=c18 $(objectCompilerFlags) msh.o msh.c

signalHandlers.o: signalHandlers.c
	gcc --std=c18 $(objectCompilerFlags) signalHandlers.o signalHandlers.c
