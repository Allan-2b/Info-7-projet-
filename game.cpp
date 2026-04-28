#include "game.hpp"
#include "mask.hpp"
#include "board.hpp"
#include "types.hpp"
#include "view.hpp"
#include "historique.hpp"
#include <iostream>
#include <random>
using namespace std;


/*Test si un mouvement est valide*/
bool test_run(Plateau P,int index,int index1){
    Masque M;
    empty_mask(&M);
    highlights_possible_moves(P, &M,P.Tab[index]);
    if(M.Tab[index1] == 1 or M.Tab[index1] == 2)return true;
    else{
        return false;
    }
    
}

void compute_score(game *G){
    /*fonction qui calcule le score du joueur en fonction des pièces prises et de la position des pièces sur le plateau*/
    G->score[0] = 39;
    G->score[1] = 39;
    for(int i = 0; i < 64; i++){
        if(G->P.Tab[i].contenu != Vide){
            if(G->P.Tab[i].contenu % 2 == 0){
                if(G->P.Tab[i].contenu == PB){
                    G->score[0] += -1;
                }
                else if(G->P.Tab[i].contenu == NB or G->P.Tab[i].contenu == BB){
                    G->score[0] += -3;
                }
                else if(G->P.Tab[i].contenu == RB ){
                    G->score[0] += -5;
                }
                else if(G->P.Tab[i].contenu == QB){
                    G->score[0] += -9;
                }  
            }
            else{
                if(G->P.Tab[i].contenu % 2 == 0){
                    if(G->P.Tab[i].contenu == PW){
                        G->score[0] += -1;
                    }
                    else if(G->P.Tab[i].contenu == NW or G->P.Tab[i].contenu == BW){
                        G->score[0] += -3;
                    }
                    else if(G->P.Tab[i].contenu == RW ){
                        G->score[0] += -5;
                    }
                    else if(G->P.Tab[i].contenu == QW){
                        G->score[0] += -9;
                    }  
                }
            }
        }
    }

}


bool est_nulle(game G){
    /*condition 2 : matériel insuffisant pour mater*/
    int pieces_non_roi_blanches = 0;
    int pieces_non_roi_noires = 0;

    for(int i = 0; i < 64; i++){
        pieces_type piece = G.P.Tab[i].contenu;

        if(piece == Vide or piece == KW or piece == KB){
            continue;
        }

        if(piece == PB or piece == PW or piece == RB or piece == RW or piece == QB or piece == QW){
            return false;
        }

        if(piece % 2 == 0){
            pieces_non_roi_blanches++;
        }
        else{
            pieces_non_roi_noires++;
        }
    }

    if (pieces_non_roi_blanches <= 1 and pieces_non_roi_noires <= 1){
        return true;
    }

    /*condition 1 : il n'y a plus de mouvements possibles pour le joueur courant et son roi n'est pas en échec*/
    if (king_in_check(G.P, 0) == false and king_in_check(G.P, 1) == false){
        Masque M;
        empty_mask(&M);
        bool mouvement_possible = false;

        highlights_attacked_pieces(G.P, &M, &M, 0);
        for(int i = 0; i < 64; i++){
            if(G.P.Tab[i].contenu % 2 == 0 and G.P.Tab[i].contenu != Vide){ /* seulement pieces blanches */
                if(can_move(G.P, G.P.Tab[i]) == true){
                    mouvement_possible = true;
                }
            }
        }

        if (mouvement_possible == false){
            highlights_attacked_pieces(G.P, &M, &M, 1);
            for(int i = 0; i < 64; i++){
                if(G.P.Tab[i].contenu % 2 == 1 and G.P.Tab[i].contenu != Vide){ /* seulement pieces noires */
                    if(can_move(G.P, G.P.Tab[i]) == true){
                        mouvement_possible = true;
                    }
                }
            }

            if (mouvement_possible == false){
                return true;
            }
        }
    }

    /*condition 3 : 3 répétitions du même positionnement */
    if (G.hist != NULL){
        historique* temp = G.hist;
        int repetition = 1;
        while (temp->suivant != NULL){
            if (temp->coord_depart == G.hist->coord_depart and temp->coord_arrivee == G.hist->coord_arrivee and temp->piece_deplacee == G.hist->piece_deplacee){
                repetition++;
            }
            temp = temp->suivant;
        }
        if (repetition >= 3){
            return true;
        }
    }
    
    /*condition 4 : 50 coups sans mouvement de pion ni capture */
    if (G.hist != NULL){
        historique* temp = G.hist;
        int coups_sans_pion_ou_capture = 0;
        while (temp != NULL){
            if (temp->piece_deplacee == PB or temp->piece_deplacee == PW or temp->piece_prise != Vide){
                break;
            }
            coups_sans_pion_ou_capture++;
            temp = temp->suivant;
        }
        if (coups_sans_pion_ou_capture >= 50){
            return true;
        }
    }

    return false;

}



    


/*Fonction intermédiaire complémentaire pour l'implantation dans choose_movement_computer pour tester un coup et le mettre dans un plateau temporaire*/
Plateau test_coup(Plateau P,int index,int index1){
    move_piece(&P,P.Tab[index],P.Tab[index1]);
    return P;
}

/*Test si le roi est en échec*/
bool king_in_check(Plateau P, int couleur_joueur){
    Masque M;
    empty_mask(&M);
    highlights_attacked_pieces(P, &M, &M, couleur_joueur);
    /*boucle qui parcourt toutes les cases du plateau et vérifie si le roi est attaqué  (en rouge)*/
    for(int i = 0; i < 64; i++){
        if (couleur_joueur == 0){
          if(P.Tab[i].contenu == KW and get_mask(M, P.Tab[i]) == 2){
              return true;
          }
        }
        if(couleur_joueur == 1){
            if(P.Tab[i].contenu == KB and get_mask(M, P.Tab[i]) == 2){
              return true;
          }
        }
    }
    return false;
}


/*Choisit le mouvement pour le joueur humain*/
void choose_movement_human(game *G,int couleur_joueur,int*choix_joueur){

    empty_mask(&G->M);
    G->couleur_joueur = couleur_joueur;
    cout<<"Entrez la coordonnée de la pièce à déplacer (ex: e2) : ";
    string coord;
    cin>>coord;
    if(coord.length() != 2 or coord[0] < 'a' or coord[0] > 'h' or coord[1] < '1' or coord[1] > '8'){
        cout<<"Coordonnée invalide. Veuillez réessayer."<<endl;
        choose_movement_human(G,couleur_joueur,choix_joueur);
        return;
    }
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
    if (king_in_check(test_coup(G->P, index, index1), couleur_joueur) == true){
        cout<<"Vous ne pouvez pas déplacer cette pièce car votre roi serait en échec."<<endl;
        choose_movement_human(G,couleur_joueur,choix_joueur);
        return;
    }

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
        or test_run(G->P, piece_a_deplacer, piece_a_deplacer_destination) == false
        or king_in_check(test_coup(G->P, piece_a_deplacer, piece_a_deplacer_destination), couleur_ordinateur) == true){ 
        /* on vérifie que la pièce choisie n'est pas vide, qu'elle est de la bonne couleur, que le mouvement est valide et que le roi n'est pas en échec après le mouvement */
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
        compute_score(G);
        print_board(G->P);
        cout << "votre score :" << G->score[G->couleur_joueur] << endl;
        cout << "score adversaire :" << G->score[1 - G->couleur_joueur] << endl;
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
    /* la couleur du joueur est blanche*/
    if (G->couleur_joueur == 0){
        one_run_human(G, NULL);
        one_run_computer(G);
    }
    /* la couleur du joueur est noire */
    else {
        one_run_computer(G);
        one_run_human(G, NULL);
    }
    

    
}

/* lance le jeu */
void run(game *G){
    start(&G->P);
    print_board(G->P);
    Masque M1;
    empty_mask(&M1);
    G->M = M1;
    G->hist = nullptr;

    for (int i = 0; i < 32; i++){
        G->prise[i] = 0;
    }

    /*Gestion de la sauvegarde*/
    cout << "Voulez vous reprendre une partie sauvegardée ? (1 pour oui, 0 pour non) : ";
    int save_choice;
    cin >> save_choice;
    if (save_choice == 1) {
        string fen;
        cout << "Entrez le nom du fichier de sauvegarde (ex: sauvegarde.txt) : ";
        cin >> fen;
        read_FEN(fen, &G->P);
        print_board(G->P);

        if (est_nulle(*G)){
            cout << "Partie nulle" << endl;
            return;
        }
    }

    /*Choix de la couleur*/
    cout << "Choisissez votre couleur (0 pour blanc, 1 pour noir) : ";
    cin >> G->couleur_joueur;
    int compteur = 0; 
    bool choix = false;

    if (est_nulle(*G)){
        cout << "Partie nulle";
        return;
    }
    /*Boucle de continuation du jeu*/
    while(choix != true and compteur < 9999999 ){
        cout << "Voulez-vous continuer à jouer ? (1 pour oui, 0 pour non) : "<< endl;
        cin >> choix;
        if (choix == 0) {
            cout << "voulez vous sauvegarder la partie ? (1 pour oui, 0 pour non) : ";
            int save_choice;
            cin >> save_choice;
            if (save_choice == 1) {
                string fen;
                cout << "Entrez le nom du fichier de sauvegarde (ex: sauvegarde.txt) : ";
                cin >> fen;
                write_fen(fen, G->P);
                cout << "Partie sauvegardée" << endl;
                return;
            }
            return;
        }
        else {
            choix = false;
        }
        cout << "voulez vous revenir en arrière ? (1 pour oui, 0 pour non) : ";
        int backtrack_choice;
        cin >> backtrack_choice;
        if (backtrack_choice == 1) {
             cout << "de combien de coups voulez vous revenir en arrière ? (0 pour non) : ";
             int backtrack_steps;
             cin >> backtrack_steps;
             if (backtrack_steps > 0) {
                 backtrack_historique(G, backtrack_steps);
             }
             else{
                 cout << "continuation du jeu." << endl;
             }
        }
        else {
            cout << "continuation du jeu." << endl;
        }

        one_run(G);

        if (est_nulle(*G)){
            cout << "Partie nulle" << endl;
            return;
        }

        compteur++;
    }
}
