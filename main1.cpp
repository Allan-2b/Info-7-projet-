#include <iostream>
#include <cstdio>
#include <cstdlib>


/*
#ifdef _WIN32
#include <windows.h>
#endif
*/

#include "board.hpp"
#include "types.hpp"
#include "view.hpp"


using namespace std;

int main(){
    
    /*
    #ifdef _WIN32
    SetConsoleOutputCP(65001); 
    #endif
    */
    
    Plateau P;
    start(&P);
    print_board(P);
    FILE *fichier = fopen("test.csv", "w");
    fprintf(fichier, "test");

    fclose(fichier);
    cout << endl;
    move_piece(&P, P.Tab[11], P.Tab[19]);
    print_board(P);
    
    
    return 0;
}
