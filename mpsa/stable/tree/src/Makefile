include ../etc/MakeSymbols

SOURCE_OBJECTS = tree_Commands.o tree_Init.o tree_TreeOps.o tree_HashOps.o $(MPSA_SRC_DIR)/mpsa.so

.suffixes : .o .c
.c.o : 
	$(CC) $(MPSA_COMPILER_FLAGS) -c $(MPSA_INCLUDE) $<
tree.so : $(SOURCE_OBJECTS)
	$(CC) -o tree.so $(MPSA_LINKER_FLAGS) $(MPSA_INCLUDE) $(MPSA_LIB) $(SOURCE_OBJECTS)
	mv tree.so ../bin
