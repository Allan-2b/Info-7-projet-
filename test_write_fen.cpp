#include <iostream>
#include <fstream>
#include "board.hpp"
#include "view.hpp"
using namespace std;

/*écris dans un fichier (write_fen) la représentation Fen d'un plateau de base (sans aucun mouvement préalable) */
int main(int argc, char* argv[]){
    if (argc != 2){
        cerr << "Usage: " << argv[0] << " <fichier_fen>" << endl;
        return 1;
    }

    Plateau P;
    start(&P);
    write_fen(argv[1], P);
    return 0;
}   