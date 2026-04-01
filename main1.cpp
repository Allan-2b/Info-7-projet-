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
    
    /* Test des pieces attaquees */
    Plateau P;
    Masque Menu;
    start(&P);
    empty_mask(&Menu);
    mask_choices_menu(P, &Menu, 0);



    FILE *fichier = fopen("test.csv", "w");
    fprintf(fichier, "test");
    fclose(fichier);

    cout << endl;
    
    return 0;
}