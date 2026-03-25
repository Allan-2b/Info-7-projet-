#ifndef VIEW_HPP_ 
#define VIEW_HPP_ 
#include <string>
#include "types.hpp"
using namespace std;


// définitons et déclaration de fonctions / types

void print_square(Case c);
void print_board(Plateau P);
void print_square_color(Case c);
void set_background(Case c);
string write_fen(string fopen, Plateau P);
void read_FEN(const string& fopen, Plateau P);

#endif 