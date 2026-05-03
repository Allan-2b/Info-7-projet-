#include <iostream>



//#ifdef _WIN32
//#include <windows.h>
//#endif


#include "board.hpp"
#include "types.hpp"
#include "view.hpp"
#include "mask.hpp"
#include "game.hpp"


using namespace std;

int main(){
    
    /* Initialisation de la console pour windows */
    //#ifdef _WIN32
    //SetConsoleOutputCP(65001); 
    //#endif
    
    game G;
    empty_mask(&G.M);
    run(&G);
    
    return 0;
}