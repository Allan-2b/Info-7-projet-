#include "game.hpp"
#include "mask.hpp"
#include "board.hpp"
#include "types.hpp"
#include "view.hpp"
#include <iostream>
#include <random>
using namespace std;




/* sous fonction pour tester si un mouvement est valide */
bool test_run(Plateau P, Masque M, int index1, int index2){

}


/* sous fonction pour choisir le mouvement de l'humain */
int choose_movement_human(game *G){

}


/* sous fonction pour choisir le mouvement de l'ordinateur */
void choose_movement_computer(game *G, int *Tab){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 63);
    int piece_a_deplacer = dis(gen);
    int piece_a_deplacer_destination = dis(gen);
    int* Tab = new int[2];
    while(G->P.Tab[piece_a_deplacer].contenu == Vide or test_run(G->P, G->M, piece_a_deplacer, piece_a_deplacer_destination) == false){
        piece_a_deplacer = dis(gen);
        piece_a_deplacer_destination = dis(gen);
    }   
    Tab[0] = piece_a_deplacer;
    Tab[1] = piece_a_deplacer_destination;

}


/* effectue un tour de jeu pour le joueur humain */
void one_run_human(game *G, int *mouvement_humain){

    cout << "C'est à vous de jouer" << endl;
    cout << "voulez vous ouvrir le menu ? (1 pour oui, 0 pour non) : ";
    int choix;
    cin >> choix;
    bool choix_valide = false;
    while (!choix_valide) {
        cout << "Veuillez entrer un choix valide (1 pour oui, 0 pour non) : ";
        cin >> choix;
        if (choix == 1 or choix == 0) {
            choix_valide = true;
        }
    }
    if (choix == 1) {
        mask_choices_menu(G->P, &G->M, G->couleur_joueur);
    }
    if (choix == 0) {
        print_board(G->P);
    }
    
    if (mouvement_humain == NULL){
        choose_movement_human(G);
    }

    move_piece(&G->P, G->P.Tab[mouvement_humain[0]], G->P.Tab[mouvement_humain[1]]);
    if (G->P.Tab[mouvement_humain[1]].contenu != Vide){
        G->prise[G->P.Tab[mouvement_humain[1]].contenu]++; /* on ajoute la piece prise dans le tableau de prise */
    }

    print_board(G->P);


}


/* effectue un tour de jeu pour l'ordinateur */
void one_run_computer(game *G){
    cout << "C'est au tour de l'ordinateur de jouer" << endl;
    print_board(G->P);
    int *Tab = new int[2];
    choose_movement_computer(G, Tab);
    move_piece(&G->P, G->P.Tab[Tab[0]], G->P.Tab[Tab[1]]);
    if (G->P.Tab[Tab[1]].contenu != Vide){
        for (int i = 0; i < 32; i++){
            if (G->P.Tab[Tab[i]].contenu == Vide){ /*Si il y a de la place*/
                G->prise[G->P.Tab[Tab[1]].contenu]++; /* on ajoute la piece prise dans le tableau de prise */   
            }
        }
    }
    print_board(G->P);
}


/* effectue un tour de jeu */
void one_run(game *G){
    if (G->couleur_joueur == 0){
        one_run_human(G, NULL);
        one_run_computer(G);
    }
    else {
        one_run_computer(G);
        one_run_human(G, NULL);
    }
}

/* lance le jeu */
void run(game *G){
    start(&G->P);
    G->M = Masque();

    for (int i = 0; i < 32; i++){
        G->prise[i] = 0;
    }
    cout << "Choisissez votre couleur (0 pour blanc, 1 pour noir) : ";
    cin >> G->couleur_joueur;
    int compteur = 0; //50 coup maximum
    while(true and compteur < 50){
        one_run(G);
        compteur++;
    }
}
