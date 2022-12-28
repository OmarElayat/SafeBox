include makefile.vars

$(BIN)/safebox: $(OBJECTS)/safebox_app.o \
		$(OBJECTS)/CommandLineArguments.o \
		$(OBJECTS)/FileSpooler.o \
		$(OBJECTS)/Block.o \
		$(OBJECTS)/SafeBox.o \
		$(OBJECTS)/ShredManager.o \
		$(OBJECTS)/Shred.o	\
		$(OBJECTS)/Thread.o	\
		$(OBJECTS)/ThreadManager.o	\
		$(OBJECTS)/Lottery.o	\
		$(OBJECTS)/ShredThread.o
	$(GCC) $(DEBUG) $(OBJECTS)/*.o -o $(BIN)/safebox $(LINKER_LIBS)

$(OBJECTS)/Block.o: $(SOURCES)/Block.cpp  $(HEADERS)/Block.h
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/Block.cpp -o $(OBJECTS)/Block.o

$(OBJECTS)/Lottery.o: $(SOURCES)/Lottery.cpp  $(HEADERS)/Lottery.h
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/Lottery.cpp -o $(OBJECTS)/Lottery.o

$(OBJECTS)/ShredManager.o: $(SOURCES)/ShredManager.cpp $(HEADERS)/ShredManager.h
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/ShredManager.cpp -o $(OBJECTS)/ShredManager.o

$(OBJECTS)/Shred.o: $(SOURCES)/Shred.cpp $(HEADERS)/Shred.h
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/Shred.cpp -o $(OBJECTS)/Shred.o

$(OBJECTS)/FileSpooler.o: $(SOURCES)/FileSpooler.cpp $(HEADERS)/FileSpooler.h
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/FileSpooler.cpp -o $(OBJECTS)/FileSpooler.o

$(OBJECTS)/CommandLineArguments.o: $(SOURCES)/CommandLineArguments.cpp $(HEADERS)/CommandLineArguments.h
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/CommandLineArguments.cpp -o $(OBJECTS)/CommandLineArguments.o

$(OBJECTS)/SafeBox.o: $(SOURCES)/SafeBox.cpp $(HEADERS)/SafeBox.h
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/SafeBox.cpp -o $(OBJECTS)/SafeBox.o

$(OBJECTS)/Thread.o: $(SOURCES)/Thread.cpp $(HEADERS)/Thread.h
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/Thread.cpp -o $(OBJECTS)/Thread.o

$(OBJECTS)/ThreadManager.o: $(SOURCES)/ThreadManager.cpp $(HEADERS)/ThreadManager.h
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/ThreadManager.cpp -o $(OBJECTS)/ThreadManager.o

$(OBJECTS)/ShredThread.o: $(SOURCES)/ShredThread.cpp $(HEADERS)/ShredThread.h
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/ShredThread.cpp -o $(OBJECTS)/ShredThread.o

$(OBJECTS)/safebox_app.o: $(SOURCES)/safebox_app.cpp
	$(GCC) $(DEBUG) $(INCLUDES) $(GCC_FLAGS) -c $(SOURCES)/safebox_app.cpp -o $(OBJECTS)/safebox_app.o

clean:
	rm -rf ./bin/* ./objects/*

rebuild: clean $(BIN)/safebox