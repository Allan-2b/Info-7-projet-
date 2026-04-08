echecs: main1.o board.o mask.o view.o game.o historique.o
	g++ main1.o board.o mask.o view.o game.o historique.o -o echecs


main1.o: main1.cpp
	g++ -c main1.cpp

board.o: board.cpp
	g++ -c board.cpp

mask.o: mask.cpp
	g++ -c mask.cpp

view.o: view.cpp
	g++ -c view.cpp


game.o: game.cpp
	g++ -c game.cpp

historique.o: historique.cpp
	g++ -c historique.cpp