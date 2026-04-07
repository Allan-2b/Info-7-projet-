#ifndef GAME_HPP
#define GAME_HPP
#include "types.hpp"
#include "mask.hpp"
#include "board.hpp"
#include "view.hpp"


void one_run(game *G);
void one_run_human(game *G, int *mouvement_humain);
void one_run_computer(game *G);
int choose_movement_human(game *G);
int choose_movement_computer(game *G);
void run(game *G);
bool test_run(Plateau P, Masque M, int couleur_joueur);

#endif 