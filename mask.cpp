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
    
    if (c.contenu == NB or c.contenu == NW) {
        highlights_possible_moves_knight(P, M, c);
    }
        
    if (c.contenu == BB or c.contenu == BW) {
        highlights_possible_moves_bishop(P, M, c);
    }
        
    if (c.contenu == QB or c.contenu == QW) {
        highlights_possible_moves_queen(P, M, c);
    }
        
    if (c.contenu == KB or c.contenu == KW) {
        highlights_possible_moves_king(P, M, c);
    }
    
    if (c.contenu == PB or c.contenu == PW) {
        highlights_possible_moves_pawn(P, M, c);
    }
    
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
    set_mask(M, c, 5);

    for (int i = 1; i < 8; i++) {

        /*HAUT*/
        /*On vérifie que la case est bien dans le plateau et que l'on n'a pas déjà rencontré une pièce dans cette direction*/
        if (c.coordonee + haut * i >= 0 and !stop_haut) {
            int idx = c.coordonee + haut * i;
            pieces_type piece = P.Tab[idx].contenu;
            /*Si la case est vide on met en bleu*/
            if (piece == Vide) {
                set_mask(M, P.Tab[idx], 1);
            }
            /*Si pièce adverse on met en rouge*/
            else {

                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, P.Tab[idx], 2);
                }
                stop_haut = true;

            }
        }

        /*BAS*/
        /*On vérifie que la case est bien dans le plateau et que l'on n'a pas déjà rencontré une pièce dans cette direction*/
        if (c.coordonee + bas * i < 64 and !stop_bas) {
            int idx = c.coordonee + bas * i;
            pieces_type piece = P.Tab[idx].contenu;
            /*Si la case est vide on met en bleu*/
            if (piece == Vide) {
                set_mask(M, P.Tab[idx], 1);
            }
            /*Si pièce adverse on met en rouge*/
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, P.Tab[idx], 2);
                }
                stop_bas = true;
            }
        }

        /*GAUCHE*/
        /*On vérifie que la case est bien dans le plateau et que l'on n'a pas déjà rencontré une pièce dans cette direction*/
        if (c.coordonee % 8 + gauche * i >= 0 and !stop_gauche) {
            int idx = c.coordonee + gauche * i;
            pieces_type piece = P.Tab[idx].contenu;
            /*Si la case est vide on met en bleu*/
            if (piece == Vide) {
                set_mask(M, P.Tab[idx], 1);
            }
            /*Si pièce adverse on met en rouge*/
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, P.Tab[idx], 2);
                }
                stop_gauche = true;
            }
        }

        /*DROITE*/
        /*On vérifie que la case est bien dans le plateau et que l'on n'a pas déjà rencontré une pièce dans cette direction*/
        if (c.coordonee % 8 + droite * i < 8 and !stop_droite) {
            int idx = c.coordonee + droite * i;
            pieces_type piece = P.Tab[idx].contenu;
            /*Si la case est vide on met en bleu*/
            if (piece == Vide) {
                set_mask(M, P.Tab[idx], 1);
            }
            /*Si pièce adverse on met en rouge*/
            else {
                if (piece % 2 != c.contenu % 2) {
                    set_mask(M, P.Tab[idx], 2);
                }
                stop_droite = true;
            }
        }
    }
}

/* Affiche le masque des mouvements possibles pour un roi */
void highlights_possible_moves_king(Plateau P, Masque *M, Case c){
    // On colore la case 
    set_mask(M, c, 5);

    // variables pour les différentes directions
    int gauche = -1;
    int droite = 1;
    int haut = -8;
    int bas = 8;
    int diagonale_haut_droit = -7;
    int diagonale_haut_gauche = -9;
    int diagonale_bas_droit = 9;
    int diagonale_bas_gauche = 7;

    // On met toutes les directions dans un tableau pour pouvoir boucler facilement
    int directions[8] = {gauche, droite, haut, bas, 
                         diagonale_haut_droit, diagonale_haut_gauche, 
                         diagonale_bas_droit, diagonale_bas_gauche};

    int colonne = c.coordonee % 8;   // numéro de la colonne (0=a ... 7=h)

    for(int i = 0; i < 8; i++){
        int new_coord = c.coordonee + directions[i];

        // On reste bien dans le plateau (cases 0 à 63)
        if(new_coord < 0 or new_coord >= 64){
            continue;
        }

        // On interdit les mouvements qui "tournent" sur le bord (ex: de a vers h)
        bool interdit = false;

        if(directions[i] == gauche and colonne == 0) interdit = true;
        if(directions[i] == droite and colonne == 7) interdit = true;

        if(directions[i] == diagonale_haut_gauche and (colonne == 0 or c.coordonee < 8)) interdit = true;
        if(directions[i] == diagonale_haut_droit  and (colonne == 7 or c.coordonee < 8)) interdit = true;

        if(directions[i] == diagonale_bas_gauche  and (colonne == 0 or c.coordonee >= 56)) interdit = true;
        if(directions[i] == diagonale_bas_droit   and (colonne == 7 or c.coordonee >= 56)) interdit = true;

        if(interdit){
            continue;
        }

        Case nouvelle_case = P.Tab[new_coord];

        // Case vide  on met la valeur 1 (bleu foncé)
        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        // Pièce adverse on met la valeur 2 (rouge)
        else if( (c.contenu == KW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                      nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                      nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                 (c.contenu == KB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                      nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                      nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
            set_mask(M, nouvelle_case, 2);
        }
        // Si c’est une pièce de la même couleur → on ne fait rien
    }
}

/* Affiche le masque des mouvements possibles pour un fou */
void highlights_possible_moves_bishop(Plateau P, Masque *M, Case c){

    // On colore le fou lui-même en violet 
    set_mask(M, c, 5);

    // Les 4 directions diagonales du fou
    int diagonale_haut_gauche = -9;
    int diagonale_haut_droit  = -7;
    int diagonale_bas_gauche  = 7;
    int diagonale_bas_droit   = 9;

    int col = c.coordonee % 8;   // numéro de colonne du fou (0=a, 7=h)

    //  Diagonale Haut-Gauche 
    int pos = c.coordonee + diagonale_haut_gauche;
    while(pos >= 0 and pos < 64){
        if(pos % 8 >= col + 1) break;   // on a traversé le bord 

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);   // bleu = case vide
        }
        else {
            // Pièce adverse ?
            if( (c.contenu == BW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == BB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);   // rouge = capture
            }
            break;   // on s'arrête sur une pièce (même couleur ou adverse)
        }
        pos += diagonale_haut_gauche;   // on continue plus loin
    }

    // Diagonale Haut-Droite 
    pos = c.coordonee + diagonale_haut_droit;
    while(pos >= 0 and pos < 64){
        if(pos % 8 <= col - 1) break;

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        else {
            if( (c.contenu == BW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == BB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);
            }
            break;
        }
        pos += diagonale_haut_droit;
    }

    // Diagonale Bas-Gauche 
    pos = c.coordonee + diagonale_bas_gauche;
    while(pos >= 0 and pos < 64){
        if(pos % 8 >= col + 1) break;

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        else {
            if( (c.contenu == BW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == BB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);
            }
            break;
        }
        pos += diagonale_bas_gauche;
    }

    //. Diagonale Bas-Droite
    pos = c.coordonee + diagonale_bas_droit;
    while(pos >= 0 and pos < 64){
        if(pos % 8 <= col - 1) break;

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        else {
            if( (c.contenu == BW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == BB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);
            }
            break;
        }
        pos += diagonale_bas_droit;
    }
}

/* Affiche le masque des mouvements possibles pour une reine */
void highlights_possible_moves_queen(Plateau P, Masque *M, Case c){
    // On colore le fou lui-même en violet 
    set_mask(M, c, 5);

    // Les 4 directions diagonales du fou
    int diagonale_haut_gauche = -9;
    int diagonale_haut_droit  = -7;
    int diagonale_bas_gauche  = 7;
    int diagonale_bas_droit   = 9;
    int haut = -8;
    int bas = 8;
    int gauche = -1;
    int droite = 1;

    int col = c.coordonee % 8;   // numéro de colonne du fou (0=a, 7=h)

    //  Diagonale Haut-Gauche 


    int pos = c.coordonee + diagonale_haut_gauche;
    while(pos >= 0 and pos < 64){
        if(pos % 8 >= col + 1) break;   // on a traversé le bord 

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);   // bleu = case vide
        }
        else {
            // Pièce adverse ?
            if( (c.contenu == QW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == QB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);   // rouge = capture
            }
            break;   // on s'arrête sur une pièce (même couleur ou adverse)
        }
        pos += diagonale_haut_gauche;   // on continue plus loin
    }

    // Diagonale Haut-Droite 
    pos = c.coordonee + diagonale_haut_droit;
    while(pos >= 0 and pos < 64){
        if(pos % 8 <= col - 1) break;

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        else {
            if( (c.contenu == QW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == QB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);
            }
            break;
        }
        pos += diagonale_haut_droit;
    }

    // Diagonale Bas-Gauche 
    pos = c.coordonee + diagonale_bas_gauche;
    while(pos >= 0 and pos < 64){
        if(pos % 8 >= col + 1) break;

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        else {
            if( (c.contenu == QW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == QB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);
            }
            break;
        }
        pos += diagonale_bas_gauche;
    }

    //. Diagonale Bas-Droite
    pos = c.coordonee + diagonale_bas_droit;
    while(pos >= 0 and pos < 64){
        if(pos % 8 <= col - 1) break;

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        else {
            if( (c.contenu == QW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == QB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);
            }
            break;
        }
        pos += diagonale_bas_droit;
    }
    // Haut
    pos = c.coordonee + haut;
    while(pos >= 0 and pos < 64){
        if(pos % 8 <= col - 1) break;

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        else {
            if( (c.contenu == QW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == QB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);
            }
            break;
        }
        pos += haut;
    }
     // Bas
    pos = c.coordonee + bas;
    while(pos >= 0 and pos < 64){
        if(pos % 8 <= col - 1) break;

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        else {
            if( (c.contenu == QW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == QB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);
            }
            break;
        }
        pos += bas;
    }
    // Droite
    pos = c.coordonee + droite;
    while(pos >= 0 and pos < 64){
        if(pos % 8 <= col - 1) break;

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        else {
            if( (c.contenu == QW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == QB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);
            }
            break;
        }
        pos += droite;
    }
     // gauche
    pos = c.coordonee + gauche;
    while(pos >= 0 and pos < 64){
        if(pos % 8 >= col + 1) break;

        Case nouvelle_case = P.Tab[pos];

        if(nouvelle_case.contenu == Vide){
            set_mask(M, nouvelle_case, 1);
        }
        else {
            if( (c.contenu == QW and (nouvelle_case.contenu == PB or nouvelle_case.contenu == NB or
                                    nouvelle_case.contenu == BB or nouvelle_case.contenu == QB or
                                    nouvelle_case.contenu == RB or nouvelle_case.contenu == KB)) or
                (c.contenu == QB and (nouvelle_case.contenu == PW or nouvelle_case.contenu == NW or
                                    nouvelle_case.contenu == BW or nouvelle_case.contenu == QW or
                                    nouvelle_case.contenu == RW or nouvelle_case.contenu == KW)) ){
                set_mask(M, nouvelle_case, 2);
            }
            break;
        }
        pos += gauche;
    }
}

/* Affiche les mouvements possibles pour un pion */
void highlights_possible_moves_pawn(Plateau P, Masque *M, Case c){
    int direction;
    set_mask(M, c, 5); 
    if (c.contenu == PW) direction = -8;  // les pions blancs montent
    else direction = 8;                     // les pions noirs descendent

    // Mouvement de base
    int new_coord = c.coordonee + direction;
    if (new_coord >= 0 and new_coord < 64 and P.Tab[new_coord].contenu == Vide) {
        set_mask(M, P.Tab[new_coord], 1);
    }

    // Capture diagonale gauche
    int diag_gauche = c.coordonee + direction - 1;
    if (diag_gauche >= 0 and diag_gauche < 64 and c.coordonee % 8 != 0) {
        pieces_type piece = P.Tab[diag_gauche].contenu;
        if (piece != Vide and piece % 2 != c.contenu % 2) {
                    set_mask(M, P.Tab[diag_gauche], 2);
                }
        }
    
    // Capture diagonale droite
    int diag_droite = c.coordonee + direction + 1;
    if (diag_droite >= 0 and diag_droite < 64 and c.coordonee % 8 != 7) {
        pieces_type piece = P.Tab[diag_droite].contenu;
        if (piece != Vide and piece % 2 != c.contenu % 2) {
                    set_mask(M, P.Tab[diag_droite], 2);
                }
        }

    //double mouvement de départ
    if ((c.contenu == PW and c.coordonee >= 48 and c.coordonee < 56) or (c.contenu == PB and c.coordonee >= 8 and c.coordonee < 16)) {
        int double_move = c.coordonee + 2 * direction;
        if (double_move >= 0 and double_move < 64 and P.Tab[double_move].contenu == Vide and P.Tab[new_coord].contenu == Vide) {
            set_mask(M, P.Tab[double_move], 1); 
            }
    }
    //On gère pas la promotion et la prise en passant pour l'instant
}

/* Affiche les mouvements possibles pour un cavalier */
void highlights_possible_moves_knight(Plateau P, Masque *M, Case c){



    // deplacement de Base en L
    int deplacement_Haut_Gauche = -17;
    int deplacement_Haut_Droit = -15;
    int deplacement_Bas_Gauche = 15;
    int deplacement_Bas_Droit = 17;
    int deplacement_Gauche_Haut = -10;
    int deplacement_Gauche_Bas = 6;
    int deplacement_Droit_Haut = -6;
    int deplacement_Droit_Bas = 10;

    set_mask(M, c, 5);
    int colonne = c.coordonee % 8;   

    // Haut Gauche
    int new_coord = c.coordonee + deplacement_Haut_Gauche;
    if (new_coord >= 0 and new_coord < 64 and colonne > 1) {
        pieces_type piece = P.Tab[new_coord].contenu;
        if (piece == Vide) {
            set_mask(M, P.Tab[new_coord], 1);
        }
        else if (piece % 2 != c.contenu % 2) {
            set_mask(M, P.Tab[new_coord], 2);
        }

    }

    // Haut Droit
    new_coord = c.coordonee + deplacement_Haut_Droit;
    if (new_coord >= 0 and new_coord < 64 and colonne < 6) {
        pieces_type piece = P.Tab[new_coord].contenu;
        if (piece == Vide) {
            set_mask(M, P.Tab[new_coord], 1);
        }
        else if (piece % 2 != c.contenu % 2) {
            set_mask(M, P.Tab[new_coord], 2);
        }

    }

    // Bas Gauche
    new_coord = c.coordonee + deplacement_Bas_Gauche;
    if (new_coord >= 0 and new_coord < 64 and colonne >= 1) {
        pieces_type piece = P.Tab[new_coord].contenu;
        if (piece == Vide) {
            set_mask(M, P.Tab[new_coord], 1);
        }
        else if (piece % 2 != c.contenu % 2) {
            set_mask(M, P.Tab[new_coord], 2);
        }

    }

    // Bas Droit
    new_coord = c.coordonee + deplacement_Bas_Droit;
    if (new_coord >= 0 and new_coord < 64 and colonne < 7) {
        pieces_type piece = P.Tab[new_coord].contenu;
        if (piece == Vide) {
            set_mask(M, P.Tab[new_coord], 1);
        }
        else if (piece % 2 != c.contenu % 2) {
            set_mask(M, P.Tab[new_coord], 2);
        }

    }

    // Gauche Haut
    new_coord = c.coordonee + deplacement_Gauche_Haut;
    if (new_coord >= 0 and new_coord < 64 and colonne > 1) {
        pieces_type piece = P.Tab[new_coord].contenu;
        if (piece == Vide) {
            set_mask(M, P.Tab[new_coord], 1);
        }
        else if (piece % 2 != c.contenu % 2) {
            set_mask(M, P.Tab[new_coord], 2);
        }

    }

    // Gauche Bas
    new_coord = c.coordonee + deplacement_Gauche_Bas;
    if (new_coord >= 0 and new_coord < 64 and colonne > 1) {
        pieces_type piece = P.Tab[new_coord].contenu;
        if (piece == Vide) {
            set_mask(M, P.Tab[new_coord], 1);
        }
        else if (piece % 2 != c.contenu % 2) {
            set_mask(M, P.Tab[new_coord], 2);
        }

    }

    // Droit Haut
    new_coord = c.coordonee + deplacement_Droit_Haut;
    if (new_coord >= 0 and new_coord < 64 and colonne < 6) {
        pieces_type piece = P.Tab[new_coord].contenu;
        if (piece == Vide) {
            set_mask(M, P.Tab[new_coord], 1);
        }
        else if (piece % 2 != c.contenu % 2) {
            set_mask(M, P.Tab[new_coord], 2);
        }

    }

    // Droit Bas
    new_coord = c.coordonee + deplacement_Droit_Bas;
    if (new_coord >= 0 and new_coord < 64 and colonne < 6) {
        pieces_type piece = P.Tab[new_coord].contenu;
        if (piece == Vide) {
            set_mask(M, P.Tab[new_coord], 1);
        }
        else if (piece % 2 != c.contenu % 2) {
            set_mask(M, P.Tab[new_coord], 2);
        }

    }
}


bool can_move(Plateau P, Case c) { // Sous fonction pour highlight_movable_pieces pour savoir si une pièce a au moins un mouvement possible
    Masque M;
    empty_mask(&M);
    highlights_possible_moves(P, &M, c);
    for(int i = 0; i < 64; i++) {
        if(M.Tab[i] == 1 or M.Tab[i] == 2) {
            return true;
        }
    }
    return false;
}


bool can_attack(Plateau P, Case c) { // Sous fonction pour highlight_take_pieces pour savoir si une pièce a au moins une prise possible
    Masque M;
    empty_mask(&M);
    highlights_possible_moves(P, &M, c);
    for(int i = 0; i < 64; i++) {
        if(M.Tab[i] == 2) {
            return true;
        }
    }
    return false;
}

/*Masque pour voir les pièces qui peuvent être déplacées*/
void highlight_movable_pieces(Plateau P, Masque *M,int N){ // 0 pour blanc et 1 pour noir
    Masque M1;
    empty_mask(&M1);
    for(int i = 0; i < 64; i++){
        if(P.Tab[i].contenu != Vide){
            highlights_possible_moves(P, &M1, P.Tab[i]);
        }
    }
    for(int j=0; j<64; j++){
        if(can_move(P, P.Tab[j])==true and M1.Tab[j] != 0 and P.Tab[j].contenu % 2 == N){
            set_mask(M, P.Tab[j], 3);
        }
    }
}

/*Masque pour voir les pièces qui peuvent être attaquées*/
void highlight_take_pieces(Plateau P, Masque *M,int N){ // 0 pour blanc et 1 pour noir
    Masque M1;
    empty_mask(&M1);
    for(int i = 0; i < 64; i++){
        if(P.Tab[i].contenu != Vide){
            highlights_possible_moves(P, &M1, P.Tab[i]);
        }
    }
    for(int j=0; j<64; j++){
        if(can_attack(P, P.Tab[j])==true and M1.Tab[j] != 0 and P.Tab[j].contenu % 2 == N){
            set_mask(M, P.Tab[j], 4);
        }
        if(get_mask(M1, P.Tab[j]) == 2 and P.Tab[j].contenu % 2 == N+1){
            set_mask(M, P.Tab[j], 2);
        }
    }
}

/*Masque pour voir les pièces qui attaquées et non attaquées*/
void highlights_attacked_pieces(Plateau P, Masque *M, Masque *M1, int Couleur_joueur){ // 0 pour blanc et 1 pour noir
    bool attacked_by_white[64];
    bool attacked_by_black[64];

    for(int i = 0; i < 64; i++){
        attacked_by_white[i] = false;
        attacked_by_black[i] = false;
    }

    empty_mask(M1);

    /* Calcule les cases attaquees par chaque camp. */
    for(int i = 0; i < 64; i++){
        if(P.Tab[i].contenu == Vide) continue;

        empty_mask(M);
        highlights_possible_moves(P, M, P.Tab[i]);

        for(int j = 0; j < 64; j++){
            if(get_mask(*M, P.Tab[j]) == 2){
                if(P.Tab[i].contenu % 2 == 0){
                    attacked_by_white[j] = true;
                }
                else {
                    attacked_by_black[j] = true;
                }
            }
        }
    }

    /* Couleur_joueur: 0 = blanc, 1 = noir */
    if (Couleur_joueur == 0){
        for(int i = 0; i < 64; i++){
            if(P.Tab[i].contenu % 2 == 0 and P.Tab[i].contenu != Vide){ /* seulement pieces blanches */

            if(attacked_by_black[i]){
                set_mask(M1, P.Tab[i], 2); /* piece blanche attaquee */
            }
            else {
                set_mask(M1, P.Tab[i], 3); /* piece blanche non attaquee */
            }
        }
        }
    }
    else {
        for(int i = 0; i < 64; i++){
            if(P.Tab[i].contenu % 2 == 1 and P.Tab[i].contenu != Vide){ /* seulement pieces noires */

            if(attacked_by_white[i]){
                set_mask(M1, P.Tab[i], 2); /* piece noire attaquee */
            }
            else {
                set_mask(M1, P.Tab[i], 3); /* piece noire non attaquee */
            }
        }
        }
    }
}

void mask_choice(Plateau P, Masque *M,int N,int choix){ // 0 pour blanc et 1 pour noir
    Masque M1;
    empty_mask(&M1);   
    if(choix == 1){
        highlight_movable_pieces(P, M, N);
        print_board(P, *M);
    }
    else if(choix == 2){
        highlight_take_pieces(P, M, N);
        print_board(P, *M);
    }
    else if(choix == 3){
        print_board(P);
        cout<<"<Entrez la coordonnée de la pièce à déplacer (ex: e2) : ";
        string coord;
        cin>>coord;
        int col = coord[0] - 'a';
        int row = 8 - (coord[1] - '0');
        int index = row * 8 + col;
        highlights_possible_moves(P, M, P.Tab[index]);
        print_board(P, *M);
    }
    else if(choix == 4){
        highlights_attacked_pieces(P, M, &M1, N);
        print_board(P, M1);
    }
}

void mask_choices_menu(Plateau P, Masque *M, int N){ // 0 pour blanc et 1 pour noir
    bool quit = false ;
    cout<<"Choisissez une option de masque : "<<endl;
    cout<<"0. quitter"<<endl;
    cout<<"1. Afficher les pièces qui peuvent être déplacées"<<endl;
    cout<<"2. Afficher les pièces qui peuvent être attaquées"<<endl;
    cout<<"3. Afficher les mouvements possibles pour une pièce spécifique"<<endl;
    cout<<"4. Afficher les pièces attaquées et non attaquées"<<endl;
    int choix;
    cin >> choix;
    while((choix >= 0 and choix <= 4) and quit == false){
        if (choix == 0){
            quit = true;
        }
        if(!quit){
            mask_choice(P, M, N, choix);
        }
    }

}

