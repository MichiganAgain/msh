objects = main.o msh.o terminalHandler.o hashMap_linkedList.o hashMap.o environmentVariables.o aliases.o builtins.o
objectCompilerFlags = -g -c -o
libdir = library
hashMapDir = $(libdir)/datastructures/HashMap

main: $(objects)
	gcc --std=c18 -g -o main $(objects)

main.o: main.c
	gcc --std=c18 $(objectCompilerFlags) main.o main.c

msh.o: $(libdir)/msh/msh.c
	gcc --std=c18 $(objectCompilerFlags) msh.o $(libdir)/msh/msh.c

terminalHandler.o: $(libdir)/terminal/terminalHandler.c
	gcc --std=c18 $(objectCompilerFlags) terminalHandler.o $(libdir)/terminal/terminalHandler.c

hashMap_linkedList.o: $(hashMapDir)/linkedList.c
	gcc --std=c18 $(objectCompilerFlags) hashMap_linkedList.o $(hashMapDir)/linkedList.c

hashMap.o: $(hashMapDir)/hashMap.c
	gcc --std=c18 $(objectCompilerFlags) hashMap.o $(hashMapDir)/hashMap.c

environmentVariables.o: $(libdir)/msh/environmentVariables.c
	gcc --std=c18 $(objectCompilerFlags) environmentVariables.o $(libdir)/msh/environmentVariables.c

aliases.o: $(libdir)/msh/aliases.c
	gcc --std=c18 $(objectCompilerFlags) aliases.o $(libdir)/msh/aliases.c

builtins.o: $(libdir)/msh/builtins.c
	gcc --std=c18 $(objectCompilerFlags) builtins.o $(libdir)/msh/builtins.c
