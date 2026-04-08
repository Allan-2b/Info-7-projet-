#include "game.hpp"
#include "mask.hpp"
#include "board.hpp"
#include "types.hpp"
#include "view.hpp"
#include "historique.hpp"
#include <iostream>
#include <random>
using namespace std;



bool test_run(Plateau P,int index,int index1){
    Masque M;
    empty_mask(&M);
    highlights_possible_moves(P, &M,P.Tab[index]);
    if(M.Tab[index1] == 1 or M.Tab[index1] == 2)return true;
    else{
        return false;
    }
    
}


void choose_movement_human(game *G,int couleur_joueur,int*choix_joueur){
    
    empty_mask(&G->M);
    G->couleur_joueur = couleur_joueur;
    cout<<"Entrez la coordonnée de la pièce à déplacer (ex: e2) : ";
    string coord;
    cin>>coord;
    int col = coord[0] - 'a';
    int row = 8 - (coord[1] - '0');
    int index = row * 8 + col;
    choix_joueur[0] = index;
    if (G->P.Tab[choix_joueur[0]].contenu % 2 != couleur_joueur or G->P.Tab[choix_joueur[0]].contenu == Vide){
        cout<<"Veuillez choisir une pièce de votre couleur."<<endl;
        choose_movement_human(G,couleur_joueur,choix_joueur);
        return;
    }
    highlights_possible_moves(G->P, &G->M, G->P.Tab[index]);
    print_board(G->P, G->M);
    cout<<"Entrez la coordonnée de la case de destination (ex: e4) : ";
    string coord1;
    cin>>coord1;
    int col1 = coord1[0] - 'a';
    int row1 = 8 - (coord1[1] - '0');
    int index1 = row1 * 8 + col1;
    if(test_run(G->P,index,index1)==false){
        choose_movement_human(G,couleur_joueur,choix_joueur);
    }
    else{
        choix_joueur[0] = index;
        choix_joueur[1] = index1;
    }

}


/* sous fonction pour choisir le mouvement de l'ordinateur */
void choose_movement_computer(game *G, int *Tab, int couleur_joueur){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 63);
    int couleur_ordinateur = 1 - couleur_joueur;
    int piece_a_deplacer = dis(gen);
    int piece_a_deplacer_destination = dis(gen);
    while (G->P.Tab[piece_a_deplacer].contenu == Vide
        or (G->P.Tab[piece_a_deplacer].contenu % 2) != couleur_ordinateur
        or test_run(G->P, piece_a_deplacer, piece_a_deplacer_destination) == false){
        piece_a_deplacer = dis(gen);
        piece_a_deplacer_destination = dis(gen);
    }
    Tab[0] = piece_a_deplacer;
    Tab[1] = piece_a_deplacer_destination;

}


/* effectue un tour de jeu pour le joueur humain */
void one_run_human(game *G, int *mouvement_humain){
    int choix_joueur[2];
    cout << "C'est à vous de jouer" << endl;
    int choix;
    bool choix_valide = false;
    while (!choix_valide) {
        cout << "Voulez-vous ouvrir le menu ? (1 pour oui, 0 pour non) : ";
        cin >> choix;
        if (choix == 1 or choix == 0) {
            choix_valide = true;
        }
    }

    if (choix == 1) {
        mask_choices_menu(G->P, &G->M, G->couleur_joueur);
    }
    else {
        print_board(G->P);
    }

    if (mouvement_humain == NULL){
        choose_movement_human(G, G->couleur_joueur, choix_joueur);
        mouvement_humain = choix_joueur;
    }

    pieces_type piece_arrivee = G->P.Tab[mouvement_humain[1]].contenu;
    move_piece(&G->P, G->P.Tab[mouvement_humain[0]], G->P.Tab[mouvement_humain[1]]);
    if (piece_arrivee != Vide){
        G->prise[piece_arrivee]++; /* on ajoute la piece prise dans le tableau de prise */
    }
    MAJ_historique(G, mouvement_humain[0], mouvement_humain[1], G->P.Tab[mouvement_humain[1]].contenu, piece_arrivee); /* on met à jour l'historique du jeu */
    print_board(G->P);
}


/* effectue un tour de jeu pour l'ordinateur */
void one_run_computer(game *G){
    cout << "C'est au tour de l'ordinateur de jouer" << endl;
    int Tab[2];
    choose_movement_computer(G, Tab, G->couleur_joueur);
    pieces_type piece_arrivee = G->P.Tab[Tab[1]].contenu;
    move_piece(&G->P, G->P.Tab[Tab[0]], G->P.Tab[Tab[1]]);
    if (piece_arrivee != Vide){
        G->prise[piece_arrivee]++; /* on ajoute la piece prise dans le tableau de prise */
    }
    MAJ_historique(G, Tab[0], Tab[1], G->P.Tab[Tab[1]].contenu, piece_arrivee); /* on met à jour l'historique du jeu */
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
    print_board(G->P);
    G->M = Masque();
    G->hist = nullptr;

    for (int i = 0; i < 32; i++){
        G->prise[i] = 0;
    }
    cout << "Choisissez votre couleur (0 pour blanc, 1 pour noir) : ";
    cin >> G->couleur_joueur;
    int compteur = 0; //50 coup maximum
    bool choix = false;
    while(choix != true and compteur < 50){
        cout << "Voulez-vous continuer à jouer ? (1 pour oui, 0 pour non) : "<< endl;
        cin >> choix;
        if (choix == 0) {
            choix = true;
            break; 
        }
        else {
            choix = false;
        }
        cout << "voulez vous revenir en arrière ? (1 pour oui, 0 pour non) : ";
        int backtrack_choice;
        cin >> backtrack_choice;
        cout << "de combien de coups voulez vous revenir en arrière ? (0 pour non) : ";
        int backtrack_steps;
        cin >> backtrack_steps;
        if (backtrack_choice == 1 and backtrack_steps > 0) {
            backtrack_historique(G, backtrack_steps);

        }
        else{
            cout << "valeur invalide, continuation du jeu." << endl;
        }

        one_run(G);
        compteur++;
    }
}
