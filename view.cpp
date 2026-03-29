#include <iostream>
#include <fstream>
#include <random>
#include <math.h>
#include <cmath>
#include <cfloat>
/*#include <windows.h>*/
#include "board.hpp"
#include "types.hpp"
#include "view.hpp"
using namespace std;



void print_square(Case c){
    switch (c.contenu) {
        case PB:
            cout << "♟";
            break;
        case PW:
            cout << "♙";
            break;
        case RB:
            cout << "♜";
            break;
        case RW:
            cout << "♖";
            break;
        case NB:
            cout << "♞";
            break;
        case NW:
            cout << "♘";
            break;
        case BB:
            cout << "♝";
            break;
        case BW:
            cout << "♗";
            break;
        case QB:
            cout << "♛";
            break;
        case QW:
            cout << "♕";
            break;
        case KB:
            cout << "♚";
            break;
        case KW:
            cout << "♔";
            break;
        case Vide:
            cout << " ";
            break;
    }

}

void set_background(Case c){
    if ((c.coordonee / 8 + c.coordonee % 8) % 2 == 0) {
        cout << "\x1b[48;5;180m";
    } else {
        cout << "\x1b[48;5;223m";
    }
}
void set_foreground(Case c){
    if (c.contenu == PW or c.contenu == RW or c.contenu == NW or
        c.contenu == BW or c.contenu == QW or c.contenu == KW) {
        cout << "\x1b[38;5;15m";
    }
    else {
        cout << "\x1b[38;5;16m";
    }
}
void print_square_color(Case c){
    set_background(c);
    set_foreground(c);
    cout << " ";
    print_square(c);
    cout << " ";
    cout << "\x1b[0m";
}




void print_board(Plateau P){
    cout << "\n   ";
    for (int col = 0; col < 8; col++) {
        cout << " " << static_cast<char>('a' + col) << " ";
    }
    cout << endl;
    cout << "  --------------------------" << endl;

    for (int ligne = 0; ligne < 8; ligne++){
        cout << (8 - ligne) << " |"; 
        for (int col = 0; col < 8; col++){
            int i = ligne * 8 + col;
            print_square_color(P.Tab[i]);
            if (col == 7) {
                cout << " |" << (8 - ligne) << endl;
            }
        }
    }
    cout << "  --------------------------" << endl;
    cout << "   ";
    for (int col = 0; col < 8; col++) {
        cout << " " << static_cast<char>('a' + col) << " ";
    }
    cout << endl;
}

string write_fen(string fopen, Plateau P){
    string fen = "";
    for (int ligne = 0; ligne < 8; ligne++) {
        int compteur_vide = 0;
        for (int col = 0; col < 8; col++) {
            int i = ligne * 8 + col;
            if (P.Tab[i].contenu == Vide) {
                compteur_vide++;
            } else {
                if (compteur_vide > 0) {
                    fen += to_string(compteur_vide);
                    compteur_vide = 0;
                }

                if (P.Tab[i].contenu == PB) fen += "p";
                else if (P.Tab[i].contenu == PW) fen += "P";
                else if (P.Tab[i].contenu == RB) fen += "r";
                else if (P.Tab[i].contenu == RW) fen += "R";
                else if (P.Tab[i].contenu == NB) fen += "n";
                else if (P.Tab[i].contenu == NW) fen += "N";
                else if (P.Tab[i].contenu == BB) fen += "b";
                else if (P.Tab[i].contenu == BW) fen += "B";
                else if (P.Tab[i].contenu == QB) fen += "q";
                else if (P.Tab[i].contenu == QW) fen += "Q";
                else if (P.Tab[i].contenu == KB) fen += "k";
                else if (P.Tab[i].contenu == KW) fen += "K";
            }
        }

        if (compteur_vide > 0) {
            fen += to_string(compteur_vide);
        }
        if (ligne < 7) {
            fen += "/";
        }
    }

    ofstream fic(fopen.c_str());
    if (fic) {
        fic << fen << endl;
        fic.close();
    }
    return fen;

}

void read_FEN(const string& fopen, Plateau* P){
    ifstream fic(fopen.c_str());
    if (fic) {
        empty(P);
        string fen;
        fic >> fen;
        fic.close();

        int index = 0;
        for (char c : fen) {
            if (c == '/') { // On parcourt chaques caractères de la chaîne FEN, si on rencontre un '/' on
                continue; 
            } 

            else if (c >= '1' and c <= '8') {
                int cases_vides = c - '0'; 
                for (int k = 0; k < cases_vides and index < 64; k++){
                    P->Tab[index].contenu = Vide;
                    index++;
                }
            } else {
                switch (c) {
                    case 'p':
                        P->Tab[index].contenu = PB;
                        break;
                    case 'P':
                        P->Tab[index].contenu = PW;
                        break;
                    case 'r':
                        P->Tab[index].contenu = RB;
                        break;
                    case 'R':
                        P->Tab[index].contenu = RW;
                        break;
                    case 'n':
                        P->Tab[index].contenu = NB;
                        break;
                    case 'N':
                        P->Tab[index].contenu = NW;
                        break;
                    case 'b':
                        P->Tab[index].contenu = BB;
                        break;
                    case 'B':
                        P->Tab[index].contenu = BW;
                        break;
                    case 'q':
                        P->Tab[index].contenu = QB;
                        break;
                    case 'Q':
                        P->Tab[index].contenu = QW;
                        break;
                    case 'k':
                        P->Tab[index].contenu = KB;
                        break;
                    case 'K':
                        P->Tab[index].contenu = KW;
                        break;
                }
                if (index < 64){
                    index++;
                }
            }
        }
    }

}