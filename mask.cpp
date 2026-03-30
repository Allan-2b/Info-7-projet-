#include "mask.hpp"
#include "view.hpp"
#include "board.hpp"
#include <iostream>
using namespace std;


/* vide le masque */
void empty_mask(Masque* M){
    for(int i = 0; i < 64; i++){
        M->Tab[i] = 0;
    }
}

/* efface le masque */
void clear_mask(Masque* M){
    empty_mask(M);
}

/* récupère la valeur d'une case du masque */
int get_mask(Masque M, Case c){
    if (c.coordonee < 0 or c.coordonee >= 64) return 0;
    return M.Tab[c.coordonee];
}

/* définit la valeur d'une case du masque */
void set_mask(Masque* M, Case c, int valeur){
    if (c.coordonee < 0 or c.coordonee >= 64) return;
    M->Tab[c.coordonee] = valeur;
}

/* affiche une case à partir du masque */
void print_square(Masque M, Case c){
    int val = get_mask(M, c);

    if (val == 0) {
        print_square_color(c);           
    }
    else {

        switch(val) {
            case 1:  cout << "\x1b[48;5;33m";
			  break;  // bleu foncé
            case 2:  cout << "\x1b[48;5;196m";
			 break;  // rouge
            case 3:  cout << "\x1b[48;5;34m";
			  break;  // vert
            case 4:  cout << "\x1b[48;5;45m";
			  break;  // bleu clair
            case 5:  cout << "\x1b[48;5;57m";
			  break;  // violet
        }

        set_foreground(c);
        cout << " ";
        print_square(c);       
        cout << " ";
        cout << "\x1b[0m";
    }
}

/* affiche le plateau avec le masque */
void print_board(Plateau P, Masque M){
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
            print_square(M, P.Tab[i]);

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

/*Affiche le masque des mouvements possibles correspondants a chacune des pièces*/
void highlights_possible_moves(Plateau P, Masque* M, Case c){
    if (c.coordonee < 0 or c.coordonee >= 64) return;
    if (c.contenu == Vide) return;
    /*
    if (c.contenu == NB or c.contenu == NW) {
        highlights_possible_moves_knight(P, M, c);
    }
    if (c.contenu == BB or c.contenu == BW) {
        highlights_possible_moves_bishop(P, M, c);
    }
    if (c.contenu == QB or c.contenu == QW) {
        highlights_possible_moves_queen(P, M, c);
    }
        */
    if (c.contenu == KB or c.contenu == KW) {
        highlights_possible_moves_king(P, M, c);
    }
    /*
    if (c.contenu == PB or c.contenu == PW) {
        highlights_possible_moves_pawn(P, M, c);
    }
    */
    if (c.contenu == RB or c.contenu == RW) {
        highlights_possible_moves_rook(P, M, c);
    }
}


/* Affiche le masque des mouvements possibles pour une tour */
void highlights_possible_moves_rook(Plateau P, Masque* M, Case c){
    int haut = -8;
    int bas = 8;
    int gauche = -1;
    int droite = 1;
    bool stop_haut = false;
    bool stop_bas = false;
    bool stop_gauche = false;
    bool stop_droite = false;
    set_mask(M, {c.coordonee}, 2);

    for (int i = 1; i < 8; i++) {

        /*HAUT*/
        if (c.coordonee + haut * i >= 0 and !stop_haut) {
            int idx = c.coordonee + haut * i;
            pieces_type piece = P.Tab[idx].contenu;

            if (piece == Vide) {
                set_mask(M, {idx}, 1);
            }
            else {

                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, {idx}, 2);
                }
                stop_haut = true;

            }
        }

        /*BAS*/
        if (c.coordonee + bas * i < 64 and !stop_bas) {
            int idx = c.coordonee + bas * i;
            pieces_type piece = P.Tab[idx].contenu;

            if (piece == Vide) {
                set_mask(M, {idx}, 1);
            }
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, {idx}, 2);
                }
                stop_bas = true;
            }
        }

        /*GAUCHE*/
        if (c.coordonee % 8 + gauche * i >= 0 and !stop_gauche) {
            int idx = c.coordonee + gauche * i;
            pieces_type piece = P.Tab[idx].contenu;

            if (piece == Vide) {
                set_mask(M, {idx}, 1);
            }
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, {idx}, 2);
                }
                stop_gauche = true;
            }
        }

        /*DROITE*/
        if (c.coordonee % 8 + droite * i < 8 and !stop_droite) {
            int idx = c.coordonee + droite * i;
            pieces_type piece = P.Tab[idx].contenu;

            if (piece == Vide) {
                set_mask(M, {idx}, 1);
            }
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, {idx}, 2);
                }
                stop_droite = true;
            }
        }
    }
}



void highlights_possible_moves_king(Plateau P,Masque *M,Case c){
    set_mask(M,c,2);
    int gauche = -1;
    int droite = 1;
    int haut = -8;
    int bas = 8;
    int diagonale_haut_droit = -9;
    int diagonale_haut_gauche = -7;
    int diagonale_bas_droit = 9;
    int diagonale_bas_gauche = 7;
    bool stop_gauche = false;
    bool stop_droite = false;
    bool stop_haut = false;
    bool stop_bas = false;
    bool stop_diagonale_haut_droit = false;
    bool stop_diagonale_haut_gauche = false;
    bool stop_diagonale_bas_droit = false;
    bool stop_diagonale_bas_gauche = false;
    for(int i=0;i<8;i++){
        /*HAUT*/
        if (c.coordonee + haut >= 0 and !stop_haut) {
            int idx = c.coordonee + haut;
            pieces_type piece = P.Tab[idx].contenu;

            if (piece == Vide) {
                set_mask(M, {idx}, 1);
            }
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, {idx}, 2);
                }
                stop_haut = true;
            }
        }

        /*BAS*/
        if (c.coordonee + bas < 64 and !stop_bas) {
            int idx = c.coordonee + bas;
            pieces_type piece = P.Tab[idx].contenu;


            if (piece == Vide) {
                set_mask(M, {idx}, 1);
            }
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, {idx}, 2);
                }
                stop_bas = true;
            }
        }

        /*GAUCHE*/ 
        if (c.coordonee % 8 + gauche >= 0 and !stop_gauche) {
            int idx = c.coordonee + gauche;
            pieces_type piece = P.Tab[idx].contenu;

            if (piece == Vide) {
                set_mask(M, {idx}, 1);
            }
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, {idx}, 2);
                }
                stop_gauche = true;
            }
        }

        /*DROITE*/
        if (c.coordonee % 8 + droite < 8 and !stop_droite) {
            int idx = c.coordonee + droite;
            pieces_type piece = P.Tab[idx].contenu;

            if (piece == Vide) {
                set_mask(M, {idx}, 1);
            }
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, {idx}, 2);
                }
                stop_droite = true;
            }
        }

        /*DIAGONALE HAUT DROIT*/
        if (c.coordonee % 8 + droite < 8 and c.coordonee + haut >= 0 and !stop_diagonale_haut_droit) {
            int idx = c.coordonee + diagonale_haut_droit;
            pieces_type piece = P.Tab[idx].contenu;

            if (piece == Vide) {
                set_mask(M, {idx}, 1);
            }
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, {idx}, 2);
                }
                stop_diagonale_haut_droit = true;
            }
        }


    }
}