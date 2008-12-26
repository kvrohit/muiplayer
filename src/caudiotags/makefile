CXX = g++
LIB_PATH = ../../lib
OPT = -Wall -pedantic -c
CPP_SRC = caudiotags

#messages

MSG_CLEAN_START = Cleaning workspace...
MSG_LIB_START = Building library...
MSG_DBUG_START = Building debug version...
MSG_DONE = Done.

win_lib:
	@echo $(MSG_LIB_START)
	@$(CXX) --version	
	$(CXX) $(OPT) $(CPP_SRC).cpp -o $(LIB_PATH)/$(CPP_SRC).o	
	@echo $(MSG_DONE)

win_clean:
	@echo $(MSG_CLEAN_START)
	del $(LIB_PATH)*.o
	@echo $(MSG_DONE)
	
win_debug:
	@echo $(MSG_DBUG_START)
	@$(CXX) --version	
	$(CXX) $(OPT) -DDBUG $(CPP_SRC).cpp -o $(LIB_PATH)/$(CPP_SRC).o	
	@echo $(MSG_DONE)

linux_lib:
	@echo $(MSG_LIB_START)
	@$(CXX) --version	
	$(CXX) $(OPT) -D_NIX $(CPP_SRC).cpp -o $(LIB_PATH)/$(CPP_SRC)_linux.o	
	@echo $(MSG_DONE)

linux_clean:
	@echo $(MSG_CLEAN_START)
	rm -f $(LIB_PATH)*.o
	@echo $(MSG_DONE)
	
linux_debug:
	@echo $(MSG_DBUG_START)
	@$(CXX) --version	
	$(CXX) $(OPT) -DDBUG -D_NIX $(CPP_SRC).cpp -o $(LIB_PATH)/$(CPP_SRC)_linux.o	
	@echo $(MSG_DONE)

