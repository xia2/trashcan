include ../etc/MakeSymbols

SOURCE_OBJECTS = quadtree_Commands.o quadtree_Init.o quadtree_TreeOps.o quadtree_HashOps.o $(MPSA_SRC_DIR)/mpsa.so

.suffixes : .o .c
.c.o : 
	$(CC) $(MPSA_COMPILER_FLAGS) -c $(MPSA_INCLUDE) $<
quadtree.so : $(SOURCE_OBJECTS)
	$(CC) -o quadtree.so $(MPSA_LINKER_FLAGS) $(MPSA_INCLUDE) $(MPSA_LIB) $(SOURCE_OBJECTS)
	mv quadtree.so ../bin
