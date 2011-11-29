include ../etc/MakeSymbols
SOURCE_OBJECTS = sn_Command.o sn_Ops.o sn_Init.o $(MPSA_SRC_DIR)/mpsa.so
.suffixes : .o .c
.c.o : 
	$(CC) $(MPSA_COMPILER_FLAGS) -c $(MPSA_INCLUDE) $<
sn.so : $(SOURCE_OBJECTS)
	$(CC) -o sn.so $(MPSA_LINKER_FLAGS) $(MPSA_INCLUDE) $(MPSA_LIB) $(SOURCE_OBJECTS)
	mv sn.so ../bin
