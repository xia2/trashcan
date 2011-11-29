include ../etc/MakeSymbols

SOURCE_OBJECTS = celllist_Command.o celllist_Init.o celllist_Ops.o $(MPSA_SRC_DIR)/mpsa.so

.suffixes : .o .c
.c.o : 
	$(CC) $(MPSA_COMPILER_FLAGS) -c $(MPSA_INCLUDE) $<
celllist.so : $(SOURCE_OBJECTS)
	$(CC) -o celllist.so $(MPSA_LINKER_FLAGS) $(MPSA_INCLUDE) $(MPSA_LIB) $(SOURCE_OBJECTS)
	mv celllist.so ../bin
