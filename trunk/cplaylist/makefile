CXX = g++
LIB_PATH = ../../lib
OPT = -Wall -pedantic -c
CPP_SRC = cplaylist

#messages

MSG_CLEAN_START = Cleaning workspace...
MSG_LIB_START = Building library...
MSG_DBUG_START = Building debug version...
MSG_DONE = Done.

lib:
	@echo $(MSG_LIB_START)
	@$(CXX) --version	
	$(CXX) $(OPT) $(CPP_SRC).cpp -o $(LIB_PATH)/$(CPP_SRC).o	
	@echo $(MSG_DONE)

clean:
	@echo $(MSG_CLEAN_START)
	rm -f $(LIB_PATH)*.o
	@echo $(MSG_DONE)
	debug:
	@echo $(MSG_DBUG_START)
	@$(CXX) --version	
	$(CXX) $(OPT) -DDBUG $(CPP_SRC).cpp -o $(LIB_PATH)/$(CPP_SRC)_linux.o	
	@echo $(MSG_DONE)

