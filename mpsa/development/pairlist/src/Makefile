include ../etc/MakeSymbols

SOURCE_OBJECTS = pairlist_Command.o pairlist_Init.o pairlist_Ops.o $(MPSA_SRC_DIR)/mpsa.so

.suffixes : .o .c
.c.o : 
	$(CC) $(MPSA_COMPILER_FLAGS) -c $(MPSA_INCLUDE) $<
pairlist.so : $(SOURCE_OBJECTS)
	$(CC) -o pairlist.so $(MPSA_LINKER_FLAGS) $(MPSA_INCLUDE) $(MPSA_LIB) $(SOURCE_OBJECTS)
	mv pairlist.so ../bin
