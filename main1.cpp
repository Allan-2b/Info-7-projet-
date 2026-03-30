#include <iostream>
#include <cstdio>
#include <cstdlib>



#ifdef _WIN32
#include <windows.h>
#endif


#include "board.hpp"
#include "types.hpp"
#include "view.hpp"
#include "mask.hpp"


using namespace std;

int main(){
    
    /* Initialisation de la console pour windows */
    #ifdef _WIN32
    SetConsoleOutputCP(65001); 
    #endif
    
    /* Initialisation du plateau et d'un test de masque pour une tour ajouté en indice 40 */
    Plateau P;
    Masque M;
    Masque M2;
    start(&P);
    P.Tab[40].contenu = RW;
    P.Tab[32].contenu = KB;
    for(int i = 0; i < 64; i++){
        P.Tab[i].coordonee = i;
    }

    empty_mask(&M);
    empty_mask(&M2);
    highlights_possible_moves(P, &M, P.Tab[40]);
    highlights_possible_moves(P, &M2, P.Tab[32]);
    print_board(P, M);
    print_board(P, M2);
    
    FILE *fichier = fopen("test.csv", "w");
    fprintf(fichier, "test");
    fclose(fichier);

    cout << endl;
    
    return 0;
}