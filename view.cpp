#include <iostream>
#include <random>
#include <math.h>
#include <cmath>
#include <cfloat>
#include "board.hpp"
#include "types.hpp"
#include "view.hpp"
using namespace std;

void print_square(Case c){
    switch (c.contenu) {
        case PB:
            cout << "p";
            break;
        case PW:
            cout << "P";
            break;
        case RB:
            cout << "r";
            break;
        case RW:
            cout << "R";
            break;
        case NB:
            cout << "n";
            break;
        case NW:
            cout << "N";
            break;
        case BB:
            cout << "b";
            break;
        case BW:
            cout << "B";
            break;
        case QB:
            cout << "q";
            break;
        case QW:
            cout << "Q";
            break;
        case KB:
            cout << "k";
            break;
        case KW:
            cout << "K";
            break;
        case Vide:
            cout << "-";
            break;
    }

}


void print_board(Plateau P){
    for (int i=0; i<64; i++){
        print_square(P.Tab[i]);
        cout << "\t";
        if((i+1) % 8 == 0) cout << endl; 
    }
}


string write_fen(string fopen, Plateau P){
    (void)fopen;
    string fen = "";
    int compteur_vide = 0;
    for (int i=0; i<64; i++){
        if (P.Tab[i].contenu == Vide){
            compteur_vide++;
        }
        else {
        if (compteur_vide > 0 or compteur_vide%8 == 0) {
                fen += to_string(compteur_vide);
                compteur_vide = 0;
            }
        if (P.Tab[i].contenu == PB){
                fen += "p";
            }
            else if (P.Tab[i].contenu == PW){
            fen += "P";
        }
        else if (P.Tab[i].contenu == RB){
            fen += "r";
        }
        else if (P.Tab[i].contenu == RW){
            fen += "R";
        }
        else if (P.Tab[i].contenu == NB){
            fen += "n";
        }
        else if (P.Tab[i].contenu == NW){
            fen += "N";
        }
        else if (P.Tab[i].contenu == BB){
            fen += "b";
        }
        else if (P.Tab[i].contenu == BW){
            fen += "B";
        }
        else if (P.Tab[i].contenu == QB){
            fen += "q";
        }
        else if (P.Tab[i].contenu == QW){
            fen += "Q";
        }
        else if (P.Tab[i].contenu == KB){
            fen += "k";
        }
        else if (P.Tab[i].contenu == KW){
            fen += "K";
        } 
    }
    }
    return fen;
}