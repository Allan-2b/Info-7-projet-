#ifndef TYPES_HPP_
#define TYPES_HPP_


// définitions et déclarations de fonctions / types

enum pieces_type{KW,KB,PW,PB,NW,NB,BW,BB,QW,QB,RW,RB,Vide};

struct Case{
    int coordonee;
    pieces_type contenu;
};

struct Plateau{
    Case Tab[64];
};

struct Masque{
    int Tab[64];
};

#endif