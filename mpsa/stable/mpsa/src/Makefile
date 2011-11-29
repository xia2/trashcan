include ../etc/MakeSymbols


SOURCE_OBJECTS = mpsa_Commands.o mpsa_DefnOps.o mpsa_FileCommands.o mpsa_ParticleCommand.o mpsa_ListCommand.o mpsa_SimCommand.o mpsa_FileOps.o mpsa_Interface.o mpsa_ListCommands.o mpsa_ListOps.o mpsa_MovementCommands.o mpsa_MovementOps.o mpsa_Ops.o mpsa_Operator.o mpsa_ParticleCommands.o mpsa_ParticleExtract.o mpsa_ParticleOps.o mpsa_SimulationOps.o mpsa_TclUtils.o mpsa_Random.o

.suffixes : .o .c
.c.o :
	$(CC) $(MPSA_COMPILER_FLAGS) -c $(MPSA_INCLUDE) $<
mpsa.so : $(SOURCE_OBJECTS)
	$(CC) -o mpsa.so $(MPSA_LINKER_FLAGS) $(SOURCE_OBJECTS)
mpsa : mpsa.o mpsa_TclMain.o mpsa_Init.o mpsa.so
	$(CC) -o mpsa $(MPSA_COMPILER_FLAGS) mpsa.o mpsa_TclMain.o mpsa_Init.o mpsa.so $(MPSA_INCLUDE) $(MPSA_LIB)
	mv mpsa ../bin
