#include <iostream>
#include "board.hpp"
#include "view.hpp"
using namespace std;


int main(int argc, char* argv[]){
    if (argc != 2){
        cerr << "Usage: " << argv[0] << " <fichier_fen>" << endl;
        return 1;
    }

    Plateau P;
    read_FEN(argv[1], P);
    print_board(P);
    return 0;
}