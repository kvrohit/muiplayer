win_all:
	g++ -Wall -pedantic cfmod_test.cpp ../../lib/cfmod.o ../../lib/libfmodex.a -o ../../bin/cfmod_test.exe
	
linux_all:	
	g++ -Wall -pedantic cfmod_test.cpp ../../lib/cfmod_linux.o -lfmodex -o ../../bin/cfmod_test
	
linux_debug:	
	g++ -Wall -pedantic -g cfmod_test.cpp ../../lib/cfmod_linux.o -lfmodex -o ../../bin/cfmod_test
