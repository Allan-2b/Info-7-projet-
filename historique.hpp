#ifndef HISTORIQUE_HPP
#define HISTORIQUE_HPP
#include "types.hpp"
#include "board.hpp"
#include "mask.hpp"
#include "game.hpp"
#include "view.hpp"

void MAJ_historique(game *G, int coord_depart, int coord_arrivee, pieces_type piece_deplacee, pieces_type piece_prise);
void play_historique(game* G,historique** histo);
bool backtrack_historique(game *G, int etape);

#endif 
