include ../etc/MakeSymbols

SOURCE_OBJECTS = diagnostic_Command.o diagnostic_Init.o $(MPSA_SRC_DIR)/mpsa.so

.suffixes : .o .c
.c.o : 
	$(CC) $(MPSA_COMPILER_FLAGS) -c $(MPSA_INCLUDE) $<
diagnostic.so : $(SOURCE_OBJECTS)
	$(CC) -o diagnostic.so $(MPSA_LINKER_FLAGS) $(MPSA_INCLUDE) $(MPSA_LIB) $(SOURCE_OBJECTS)
	mv diagnostic.so ../bin
