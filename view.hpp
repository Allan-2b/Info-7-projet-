#ifndef VIEW_HPP_ 
#define VIEW_HPP_ 
#include <string>
#include "types.hpp"


// définitons et déclaration de fonctions / types

void print_square(Case c);
void print_board(Plateau P);
std::string write_fen(std::string fopen, Plateau P);
std::string read_fen(std::string fopen, Plateau P);

#endif 