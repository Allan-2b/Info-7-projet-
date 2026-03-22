//TP7 rendu scéance 1
//Lamotte Allan / Landry Enzo

#include <iostream>
#include <random>
#include <math.h>
#include <cmath>
#include <cfloat>
using namespace std;

//représentation REP2
enum pieces_type{KW,KB,PW,PB,NW,NB,BW,BB,QW,QB,RB,RW,Vide};
struct Case{
    int coordonee;
    pieces_type contenu;
};

struct Plateau{
    Case Tab[64];
};

void empty(Plateau& P) { 
    for(int i = 0; i < 64; i++){
        P.Tab[i].contenu = Vide;
    }
}

Case get_square(Plateau P, Case c){
    Case contenu = P.Tab[c.coordonee];
    return contenu;
}


void set_square(Plateau& P, Case c, pieces_type pi){
    P.Tab[c.coordonee].contenu = pi;
}

void start(Plateau& P){
    empty(P);
    for(int i = 0; i < 64; i++){
        if (i > 7 and i < 16){
            P.Tab[i].contenu = PB;
        }
        else if (i > 47 and i < 56){
            P.Tab[i].contenu = PW;
        }
        else if (i == 0 or i == 7){
            P.Tab[i].contenu = RB;
        }
        else if (i == 56 or i == 63){
            P.Tab[i].contenu = RW;
        }
        else if (i == 1 or i == 6){
            P.Tab[i].contenu = NB;
        }
        else if (i == 57 or i == 62){
            P.Tab[i].contenu = NW;
        }
        else if (i == 2 or i == 5){
            P.Tab[i].contenu = BB;
        }
        else if (i == 58 or i == 61){
            P.Tab[i].contenu = BW;
        }
        else if (i == 3){
            P.Tab[i].contenu = QB;
        }
        else if (i == 59){
            P.Tab[i].contenu = QW;
        }
        else if (i == 4){
            P.Tab[i].contenu = KB;
        }
        else if (i == 60){
            P.Tab[i].contenu = KW;
        }

}
}

void move_piece(Plateau& P, Case c1, Case c2){
    pieces_type pi = get_square(P,c1).contenu;
    pieces_type pi2 = get_square(P,c2).contenu;
    set_square(P,c2,pi);
    set_square(P,c1,pi2);
}


int main(){
    Plateau P;
    start(P);
    for (int i=0; i<64; i++){
        cout << P.Tab[i].contenu << "\t";
        if((i+1) % 8 == 0) cout << endl; 
    }
    cout << "test move" << endl;
    Case c1 = {0, RB};
    Case c2 = {16, Vide};
    move_piece(P,c1,c2);
    for (int i=0; i<64; i++){
        cout << P.Tab[i].contenu << "\t";
        if((i+1) % 8 == 0) cout << endl; 
    }

    
    

    return 0;
}