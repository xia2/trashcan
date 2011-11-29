include ../etc/MakeSymbols

SOURCE_OBJECTS = tpp_Commands.o tpp_Init.o tpp_Ops.o tpp_HashOps.o $(MPSA_SRC_DIR)/mpsa.so

.suffixes : .o .c
.c.o : 
	$(CC) $(MPSA_COMPILER_FLAGS) -c $(MPSA_INCLUDE) $<
tpp.so : $(SOURCE_OBJECTS)
	$(CC) -o tpp.so $(MPSA_LINKER_FLAGS) $(MPSA_INCLUDE) $(MPSA_LIB) $(SOURCE_OBJECTS)
	mv tpp.so ../bin
