#
#	ServerApp for infotecs academy test task
#	by KyKyPy3HuK 21.08.23
#
CXX = g++

# all build
all : serverApp.out

serverApp.out : serverMain.o Server.o
	$(CXX) serverMain.o Server.o -o serverApp.out

serverMain.o : serverMain.cpp 
	$(CXX) -c serverMain.cpp -o serverMain.o

Server.o : Server.cpp
	$(CXX) -c Server.cpp -o Server.o
# !all build

# debug build
debug : serverApp_d.out
serverApp_d.out : serverMain.cpp Server.cpp 
	$(CXX) -g serverMain.cpp Server.cpp -o serverApp_d.out
# !debug build

# clean
clean : 
	rm *.out *.o
# !clean