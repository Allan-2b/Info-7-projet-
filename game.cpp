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
    if(index < 0 or index >= 64 or index1 < 0 or index1 >= 64) {
        return false;
    }

    if(P.Tab[index].contenu == Vide) {
        return false;
    }

    if(P.Tab[index1].contenu == KW or P.Tab[index1].contenu == KB) {
        return false;
    }

    Masque M;
    empty_mask(&M);
    highlights_possible_moves(P, &M, P.Tab[index]);

    if(M.Tab[index1] == 1 or M.Tab[index1] == 2) return true;
    return false;
}

void compute_score(game *G){
    /* fonction qui calcule le score du joueur en fonction des pièces encore présentes sur le plateau */
    G->score[0] = 0;
    G->score[1] = 0;

    for(int i = 0; i < 64; i++){
        switch(G->P.Tab[i].contenu){
            case PW:
                G->score[0] += 1;
                break;
            case NW:
            case BW:
                G->score[0] += 3;
                break;
            case RW:
                G->score[0] += 5;
                break;
            case QW:
                G->score[0] += 9;
                break;
            case PB:
                G->score[1] += 1;
                break;
            case NB:
            case BB:
                G->score[1] += 3;
                break;
            case RB:
                G->score[1] += 5;
                break;
            case QB:
                G->score[1] += 9;
                break;
            default:
                break;
        }
    }
}

void promotion(game *G, int index, int choix){ 
    /* fonction qui gère la promotion d'un pion lorsqu'il arrive sur la dernière rangée */
    if(G->P.Tab[index].contenu == PW and index <= 7){
        if(choix==1){
            G->P.Tab[index].contenu = RW; /* promotion en tour blanche */
        }
        else if(choix==2){
            G->P.Tab[index].contenu = NW; /* promotion en cavalier blanc */
        }
        else if(choix==3){
            G->P.Tab[index].contenu = BW; /* promotion en fou blanc */
        }
        else{
        G->P.Tab[index].contenu = QW; 
        }
    }
    if(G->P.Tab[index].contenu == PB and index >= 56){
        if(choix==1){
            G->P.Tab[index].contenu = RB; /* promotion en tour noire */
        }
        else if(choix==2){
            G->P.Tab[index].contenu = NB; /* promotion en cavalier noir */
        }
        else if(choix==3){
            G->P.Tab[index].contenu = BB; /* promotion en fou noir */
        }
        else{
        G->P.Tab[index].contenu = QB; 
        }
    }
}

bool verif_promotion(game *G){
    /* fonction qui gère la promotion d'un pion lorsqu'il arrive sur la dernière rangée */
    for(int i = 0; i < 64; i++){
        if(G->P.Tab[i].contenu == PW and i <= 7){
            return true;
            
        }
        if(G->P.Tab[i].contenu == PB and i >= 56){
            return true;
        }
    }
    return false;
}

void lapromotion_humain(game *G){
    if(verif_promotion(G)){
        int choix;
        cout << "Votre pion peut être promu ! Choisissez la pièce de promotion (1 pour tour, 2 pour cavalier, 3 pour fou, 4 pour reine) : ";
        cin >> choix;
        while (choix < 1 or choix > 4){
            cout << "Choix invalide. Veuillez réessayer." << endl;
            cin >> choix;
        }
        for(int i = 0; i < 64; i++){
            if(G->P.Tab[i].contenu == PW and i <= 7){
                promotion(G, i, choix);
            }
            if(G->P.Tab[i].contenu == PB and i >= 56){
                promotion(G, i, choix);
            }
        }
    }
}
void lapromotion_computer(game *G){
    if(verif_promotion(G)){
        for(int i = 0; i < 64; i++){
            if(G->P.Tab[i].contenu == PW and i <= 7){
                promotion(G, i, 4); /* promotion en reine blanche */
            }
            if(G->P.Tab[i].contenu == PB and i >= 56){
                promotion(G, i, 4); /* promotion en reine noire */
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


bool king_in_checkmate(Plateau P, int couleur_joueur){
    if (!king_in_check(P, couleur_joueur)) {
        return false;
    }

    Masque M;
    empty_mask(&M);

    for (int i = 0; i < 64; i++) {
        if (P.Tab[i].contenu == Vide) {
            continue;
        }
        if (P.Tab[i].contenu % 2 != couleur_joueur) {
            continue;
        }

        empty_mask(&M);
        highlights_possible_moves(P, &M, P.Tab[i]);

        for (int j = 0; j < 64; j++) {
            if (M.Tab[j] == 1 or M.Tab[j] == 2) {
                Plateau copy = test_coup(P, i, j);
                if (!king_in_check(copy, couleur_joueur)) {
                    return false;
                }
            }
        }
    }

    return true;
}


/*fonction qui vérifie si une pièce a déjà été déplacée */
bool piece_deja_bougee(historique* tete, pieces_type piece, int coord_depart){
    while (tete != NULL) {
        if (tete->piece_deplacee == piece and tete->coord_depart == coord_depart) { 
            /*si la pièce a déjà été déplacée depuis sa position initiale*/
            return true;
        }
        tete = tete->suivant;
    }
    return false;
}

/*fonction qui vérifie si le roque est possible*/
bool roque_possible(game *G, Plateau P, Case c1, Case c2){

    /* Petit roque blanc : roi e1(60)->g1(62), tour h1(63)->f1(61) */
    if (c1.contenu == KW and c2.contenu == RW and c1.coordonee == 60 and c2.coordonee == 63
        and !piece_deja_bougee(G->hist, KW, 60) and !piece_deja_bougee(G->hist, RW, 63)
        and P.Tab[61].contenu == Vide and P.Tab[62].contenu == Vide
        and king_in_check(P, 0) == false
        and king_in_check(test_coup(P, 60, 61), 0) == false
        and king_in_check(test_coup(P, 60, 62), 0) == false) {
        // Toutes les conditions du petit roque blanc sont remplies :
        // le roi et la tour n'ont pas bougé, les cases entre eux sont vides, 
        // et le roi n'est pas en échec avant, pendant ou après le déplacement.
        return true;
    }

    /* Grand roque blanc : roi e1(60)->c1(58), tour a1(56)->d1(59) */
    if (c1.contenu == KW and c2.contenu == RW and c1.coordonee == 60 and c2.coordonee == 56
        and !piece_deja_bougee(G->hist, KW, 60) and !piece_deja_bougee(G->hist, RW, 56)
        and P.Tab[57].contenu == Vide and P.Tab[58].contenu == Vide and P.Tab[59].contenu == Vide
        and king_in_check(P, 0) == false
        and king_in_check(test_coup(P, 60, 59), 0) == false
        and king_in_check(test_coup(P, 60, 58), 0) == false) {
        return true;
    }

    /* Petit roque noir : roi e8(4)->g8(6), tour h8(7)->f8(5) */
    if (c1.contenu == KB and c2.contenu == RB and c1.coordonee == 4 and c2.coordonee == 7
        and !piece_deja_bougee(G->hist, KB, 4) and !piece_deja_bougee(G->hist, RB, 7)
        and P.Tab[5].contenu == Vide and P.Tab[6].contenu == Vide
        and king_in_check(P, 1) == false
        and king_in_check(test_coup(P, 4, 5), 1) == false
        and king_in_check(test_coup(P, 4, 6), 1) == false) {
        return true;
    }

    /* Grand roque noir : roi e8(4)->c8(2), tour a8(0)->d8(3) */
    if (c1.contenu == KB and c2.contenu == RB and c1.coordonee == 4 and c2.coordonee == 0
        and !piece_deja_bougee(G->hist, KB, 4) and !piece_deja_bougee(G->hist, RB, 0)
        and P.Tab[1].contenu == Vide and P.Tab[2].contenu == Vide and P.Tab[3].contenu == Vide
        and king_in_check(P, 1) == false
        and king_in_check(test_coup(P, 4, 3), 1) == false
        and king_in_check(test_coup(P, 4, 2), 1) == false) {
        return true;
    }

    return false;
}

bool roque(game *G, Plateau *P, Case c1, Case c2){
    /* fonction qui effectue le roque si les conditions sont remplies.
       On place roi et tour directement avec set_square et pas move_piece pour eviter des problèmes */

    if (!roque_possible(G, *P, c1, c2)) {
        return false;
    }

    /*roque basique coté blanc : roi e1->g1, tour h1->f1*/
    if(c1.contenu == KW and c2.contenu == RW and c1.coordonee == 60 and c2.coordonee == 63){
        set_square(P, P->Tab[62], KW);
        set_square(P, P->Tab[61], RW);
        set_square(P, P->Tab[60], Vide);
        set_square(P, P->Tab[63], Vide);
        return true;
    }

    /*grand roque coté blanc : roi e1->c1, tour a1->d1*/
    if (c1.contenu == KW and c2.contenu == RW and c1.coordonee == 60 and c2.coordonee == 56){
        set_square(P, P->Tab[58], KW);
        set_square(P, P->Tab[59], RW);
        set_square(P, P->Tab[60], Vide);
        set_square(P, P->Tab[56], Vide);
        return true;
    }

    /*roque basique coté noir : roi e8->g8, tour h8->f8*/
    if (c1.contenu == KB and c2.contenu == RB and c1.coordonee == 4 and c2.coordonee == 7){
        set_square(P, P->Tab[6], KB);
        set_square(P, P->Tab[5], RB);
        set_square(P, P->Tab[4], Vide);
        set_square(P, P->Tab[7], Vide);
        return true;
    }

    /*grand roque coté noir : roi e8->c8, tour a8->d8*/
    if (c1.contenu == KB and c2.contenu == RB and c1.coordonee == 4 and c2.coordonee == 0){
        set_square(P, P->Tab[2], KB);
        set_square(P, P->Tab[3], RB);
        set_square(P, P->Tab[4], Vide);
        set_square(P, P->Tab[0], Vide);
        return true;
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
    pieces_type piece_depart = G->P.Tab[index].contenu;
    if (G->P.Tab[choix_joueur[0]].contenu % 2 != couleur_joueur or G->P.Tab[choix_joueur[0]].contenu == Vide){
        cout<<"Veuillez choisir une pièce de votre couleur."<<endl;
        choose_movement_human(G,couleur_joueur,choix_joueur);
        return;
    }
    clear_mask(&G->M);
    highlights_possible_moves(G->P, &G->M, G->P.Tab[index]);
    if (piece_depart == KW or piece_depart == KB){
        highlights_king_roque(G, &G->P, G->P.Tab[index]);
    }
    print_board(G->P, G->M);
    cout<<"Entrez la coordonnée de la case de destination (ex: e4) : ";
    string coord1;
    cin>>coord1;
    int col1 = coord1[0] - 'a';
    int row1 = 8 - (coord1[1] - '0');
    int index1 = row1 * 8 + col1;

    // Convertir les mouvements de roque indiqué par le joueur: si le roi blanc va en g1, c'est vers la tour h1
    // dans les règles des échecs, le roi se déplace de 2 cases vers la tour puis on place la tour de l'autre côté du roi.
    // Donc on convertit g1 en h1 pour le petit roque et c1 en a1 pour le grand roque, et pareil pour les noirs
    if (piece_depart == KW and index == 60 and index1 == 62) {
        index1 = 63; // Petit roque blanc: convertir g1 -> h1
    }
    else if (piece_depart == KW and index == 60 and index1 == 58) {
        index1 = 56; // Grand roque blanc: convertir c1 -> a1
    }
    else if (piece_depart == KB and index == 4 and index1 == 6) {
        index1 = 7; // Petit roque noir: convertir g8 -> h8
    }
    else if (piece_depart == KB and index == 4 and index1 == 2) {
        index1 = 0; // Grand roque noir: convertir c8 -> a8
    }
    /* Vérifier si le mouvement est un roque et l'exécuter directement dans ce cas, en mettant à jour l'historique en conséquence */
    if (roque_possible(G, G->P, G->P.Tab[index], G->P.Tab[index1]) == true){
        if (roque(G, &G->P, G->P.Tab[index], G->P.Tab[index1]) == true){
            /* Mise à jour de l'historique avec les positions réelles après roque
               (et non les cases de la tour passées en paramètre) */
            if (piece_depart == KW and index == 60 and index1 == 63){
                MAJ_historique(G, 60, 62, KW, Vide); /* roi : e1 -> g1 */
                MAJ_historique(G, 63, 61, RW, Vide); /* tour : h1 -> f1 */
            }
            else if (piece_depart == KW and index == 60 and index1 == 56){
                MAJ_historique(G, 60, 58, KW, Vide); /* roi : e1 -> c1 */
                MAJ_historique(G, 56, 59, RW, Vide); /* tour : a1 -> d1 */
            }
            else if (piece_depart == KB and index == 4 and index1 == 7){
                MAJ_historique(G, 4, 6, KB, Vide); /* roi : e8 -> g8 */
                MAJ_historique(G, 7, 5, RB, Vide); /* tour : h8 -> f8 */
            }
            else if (piece_depart == KB and index == 4 and index1 == 0){
                MAJ_historique(G, 4, 2, KB, Vide); /* roi : e8 -> c8 */
                MAJ_historique(G, 0, 3, RB, Vide); /* tour : a8 -> d8 */
            }
            print_board(G->P);
            /*sert a signaler à one_run_human que le roque est déjà effectué */
            choix_joueur[0] = -1;
            choix_joueur[1] = -1;
            return;
        }
    }

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

    /* Si le roque a déjà été exécuté dans choose_movement_human (sentinelle -1),
       on ne rejoue pas de move_piece. */
    if (mouvement_humain[0] == -1 and mouvement_humain[1] == -1){
        return;
    }

    pieces_type piece_arrivee = G->P.Tab[mouvement_humain[1]].contenu;
    move_piece(&G->P, G->P.Tab[mouvement_humain[0]], G->P.Tab[mouvement_humain[1]]);
    if (piece_arrivee != Vide){
        G->prise[piece_arrivee]++; /* on ajoute la piece prise dans le tableau de prise */
    }
    // Gérer la promotion éventuelle avant d'enregistrer l'historique
    if (verif_promotion(G)){
        lapromotion_humain(G);
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
    // Gérer la promotion éventuelle avant d'enregistrer l'historique
    if (verif_promotion(G)){
        lapromotion_computer(G);
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
        empty_mask(&G->M);
        G->hist = nullptr;
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

        if (king_in_checkmate(G->P, G->couleur_joueur) == true){
            cout << "Echec et mat ! vous avez perdu" << endl;
            return;
        }

        if (king_in_checkmate(G->P, 1 - G->couleur_joueur) == true){
            cout << "Echec et mat ! vous avez gagné" << endl;
            return;
        }

        compteur++;
    }
}
