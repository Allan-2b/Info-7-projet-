#ifndef GAME_HPP
#define GAME_HPP
#include "types.hpp"
#include "mask.hpp"
#include "board.hpp"
#include "view.hpp"


void one_run(game *G);
void one_run_human(game *G, int *mouvement_humain);
void one_run_computer(game *G);
void choose_movement_human(game *G, int couleur_joueur, int *choix_joueur);
void choose_movement_computer(game *G, int *mouvement_computer, int couleur_joueur);
void run(game *G);
bool test_run(Plateau P, int index, int index1);

#endif 