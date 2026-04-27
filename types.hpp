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

struct historique{
    int coord_depart;
    int coord_arrivee;
    pieces_type piece_deplacee;
    pieces_type piece_prise;
    historique* suivant;
    

};

struct game{
    Plateau P;
    Masque M;
    int prise[32];
    int couleur_joueur; //0 blanc 1 noir
    historique* hist;
    int score[2]; //score[0] score blanc score[1] score noir
};



#endif