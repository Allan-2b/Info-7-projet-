#ifndef BOARD_HPP_ 
#define BOARD_HPP_ 
#include "types.hpp"

// définitons et déclaration de fonctions / types

void empty(Plateau* P);
Case get_square(Plateau P, Case c);
void set_square(Plateau* P, Case c, pieces_type pi);
void start(Plateau* P);
void move_piece(Plateau* P, Case c1, Case c2);





#endif 