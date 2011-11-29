include ../etc/MakeSymbols
SOURCE_OBJECTS = cloud_Command.o cloud_Init.o cloud_Ops.o $(MPSA_SRC_DIR)/mpsa.so $(TPP_SRC_DIR)/../bin/tpp.so

.suffixes : .o .c
.c.o : 
	$(CC) $(MPSA_COMPILER_FLAGS) -c $(MPSA_INCLUDE) $<
cloud.so : $(SOURCE_OBJECTS)
	$(CC) -o cloud.so $(MPSA_LINKER_FLAGS) $(MPSA_INCLUDE) $(MPSA_LIB) $(SOURCE_OBJECTS)
	mv cloud.so ../bin
