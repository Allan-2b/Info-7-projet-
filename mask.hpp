#ifndef MASK_HPP_ 
#define MASK_HPP_ 
#include "types.hpp"

// définitons et déclaration de fonctions / types

void empty_mask(Masque* M);
void clear_mask(Masque* M);
int get_mask(Masque M, Case c);
void set_mask(Masque* M, Case c, int valeur);

#endif 