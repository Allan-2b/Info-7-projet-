#include "mask.hpp"
#include "view.hpp"
#include "board.hpp"
#include "game.hpp"
#include "historique.hpp"
#include <iostream>
using namespace std;


/*Met a jour l'historique suite a un mouvement */
void MAJ_historique(game *G, int coord_depart, int coord_arrivee, pieces_type piece_deplacee, pieces_type piece_prise){
    historique* new_historique = new historique; 
    new_historique->coord_depart = coord_depart;
    new_historique->coord_arrivee = coord_arrivee;
    new_historique->piece_deplacee = piece_deplacee;
    new_historique->piece_prise = piece_prise;
    new_historique->suivant = G->hist; 
    G->hist = new_historique; /* on ajoute le nouvel historique en tête de la liste */

}

/*Joue l'historique des mouvements*/
void play_historique(game* G,historique** histo){
    if (G == nullptr or histo == nullptr or *histo == nullptr) {
        return;
    }

    historique* tete = *histo;
    while(tete != nullptr){
        cout << "Départ : " << tete->coord_depart << " Arrivée : " << tete->coord_arrivee << " Pièce prise : " << tete->piece_prise << " Pièce déplacée : " << tete->piece_deplacee << endl;
        move_piece(&G->P, G->P.Tab[tete->coord_depart], G->P.Tab[tete->coord_arrivee]);
        print_board(G->P);
        tete = tete->suivant;

    }

}


/* reviens en arrière dans l'historique 1 = 2 coups en arrière */
bool backtrack_historique(game *G, int etape){
    for (int i = 0; i < etape; i++) {
        if (G->hist == NULL) {
            cout << "Pas assez d'historique" << endl;
            return false;
        }
        for (int j = 0; j < 2; j++) { // 2 coups en arrière
            if (G->hist == NULL) {
                cout << "Pas assez d'historique" << endl;
                return false;
            }
            historique* temp = G->hist;
            G->hist = G->hist->suivant; /* on retire le dernier historique de la liste */
            move_piece(&G->P, G->P.Tab[temp->coord_arrivee], G->P.Tab[temp->coord_depart]); /* on annule le dernier mouvement */
            G->P.Tab[temp->coord_arrivee].contenu = temp->piece_prise; /* on remet la pièce prise si il y en a une */
            G->P.Tab[temp->coord_depart].contenu = temp->piece_deplacee; /* on remet la pièce déplacée */
            delete temp; /* on libère la mémoire de l'historique annulé */
        }       
    }
    print_board(G->P);
    return true;
}



