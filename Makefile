echecs: main1.o board.o mask.o view.o
	g++ main1.o board.o mask.o view.o -o echecs

main1.o: main1.cpp
	g++ -c main1.cpp

board.o: board.cpp
	g++ -c board.cpp

mask.o: mask.cpp
	g++ -c mask.cpp

view.o: view.cpp²
	g++ -c view.cpp