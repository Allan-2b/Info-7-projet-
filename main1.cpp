#include <iostream>
#include "board.hpp"
#include "types.hpp"
#include "view.hpp"
#include <cstdio>
#include <cstdlib>
using namespace std;

int main(){
    Plateau P;
    start(&P);
    print_board(P);
    FILE *fichier = fopen("test.csv", "w");
    fprintf(fichier, "test");



    cout << endl;
    move_piece(&P, P.Tab[11], P.Tab[19]);
    print_board(P);
    
    
    return 0;
}