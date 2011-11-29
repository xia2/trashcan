include ../etc/MakeSymbols
SOURCE_OBJECTS = sf_Ops.o sf_SFCommand.o sf_Init.o $(MPSA_SRC_DIR)/mpsa.so $(CLOUD_SRC_DIR)/../bin/cloud.so

.suffixes : .o .c
.c.o : 
	$(CC) $(MPSA_COMPILER_FLAGS) -c $(MPSA_INCLUDE) $<
sf.so : $(SOURCE_OBJECTS)
	$(CC) -o sf.so $(MPSA_LINKER_FLAGS) $(MPSA_INCLUDE) $(MPSA_LIB) $(SOURCE_OBJECTS)
	mv sf.so ../bin
