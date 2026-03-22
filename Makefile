echecs: main.o board.o mask.o types.o view.o
	g++ main.o board.o mask.o types.o view.o -o echecs

main.o: main.cpp
	g++ -c main.cpp

board.o: board.cpp
	g++ -c board.cpp

mask.o: mask.cpp
	g++ -c mask.cpp

types.o: types.cpp
	g++ -c types.cpp

view.o: view.cpp
	g++ -c view.cpp