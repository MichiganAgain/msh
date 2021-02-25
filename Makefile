objects = main.o msh.o signalHandlers.o linkedList.o hashMap.o
objectCompilerFlags = -g -c -o
libdir = library

main: $(objects)
	gcc --std=c18 -g -o main $(objects)

main.o: main.c
	gcc --std=c18 $(objectCompilerFlags) main.o main.c

msh.o: $(libdir)/msh/msh.c
	gcc --std=c18 $(objectCompilerFlags) msh.o $(libdir)/msh/msh.c

signalHandlers.o: $(libdir)/signal/signalHandlers.c
	gcc --std=c18 $(objectCompilerFlags) signalHandlers.o $(libdir)/signal/signalHandlers.c

linkedList.o: $(libdir)/datastructures/linkedList.c
	gcc --std=c18 $(objectCompilerFlags) linkedList.o $(libdir)/datastructures/linkedList.c

hashMap.o: $(libdir)/datastructures/hashMap.c
	gcc --std=c18 $(objectCompilerFlags) hashMap.o $(libdir)/datastructures/hashMap.c
