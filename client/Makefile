#
#	serverApp for infotecs academy test task
#	by KyKyPy3HuK	21.08.23
#
CXX = g++

# Standart build
all : clientApp.out

clientApp.out : clientMain.o Client.o
	$(CXX) clientMain.o Client.o -o clientApp.out

clientMain.o : clientMain.cpp 
	$(CXX) -c clientMain.cpp -o clientMain.o

Client.o : Client.cpp
	$(CXX) -c Client.cpp -o Client.o
# !Standart build

# debug build
debug : clientApp_d.out
clientApp_d.out : clientMain.cpp Client.cpp 
	$(CXX) -g clientMain.cpp Client.cpp -o clientApp_d.out
# !debug build

# clean
clean : 
	rm *.out *.o
# !clean