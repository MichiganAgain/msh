objects = main.o msh.o signalHandlers.o hashMap_linkedList.o hashMap.o environmentVariables.o aliases.o builtins.o
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

hashMap_linkedList.o: $(libdir)/datastructures/HashMap/linkedList.c
	gcc --std=c18 $(objectCompilerFlags) hashMap_linkedList.o $(libdir)/datastructures/HashMap/linkedList.c

hashMap.o: $(libdir)/datastructures/HashMap/hashMap.c
	gcc --std=c18 $(objectCompilerFlags) hashMap.o $(libdir)/datastructures/HashMap/hashMap.c

environmentVariables.o: $(libdir)/msh/environmentVariables.c
	gcc --std=c18 $(objectCompilerFlags) environmentVariables.o $(libdir)/msh/environmentVariables.c

aliases.o: $(libdir)/msh/aliases.c
	gcc --std=c18 $(objectCompilerFlags) aliases.o $(libdir)/msh/aliases.c

builtins.o: $(libdir)/msh/builtins.c
	gcc --std=c18 $(objectCompilerFlags) builtins.o $(libdir)/msh/builtins.c
