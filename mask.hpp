#ifndef MASK_HPP_ 
#define MASK_HPP_ 
#include "types.hpp"

void empty_mask(Masque* M);
void clear_mask(Masque* M);
int get_mask(Masque M, Case c);
void set_mask(Masque* M, Case c, int valeur);
void highlights_possible_moves(Plateau P, Masque* M, Case c);
void highlights_possible_moves_king(Plateau P, Masque* M, Case c);
void highlights_possible_moves_pawn(Plateau P, Masque* M, Case c);
void highlights_possible_moves_rook(Plateau P, Masque* M, Case c);
void highlights_possible_moves_knight(Plateau P, Masque* M, Case c);
void highlights_possible_moves_bishop(Plateau P, Masque* M, Case c);
void highlights_possible_moves_queen(Plateau P, Masque* M, Case c);


void print_square(Masque M, Case c);
void print_board(Plateau P, Masque M);

#endif