#include "mask.hpp"

void empty_mask(Masque* M){
	for(int i = 0; i < 64; i++){
		M->Tab[i] = 0;
	}
}

void clear_mask(Masque* M){
	empty_mask(M);
}

int get_mask(Masque M, Case c){
	return M.Tab[c.coordonee];
}

void set_mask(Masque* M, Case c, int valeur){
	M->Tab[c.coordonee] = valeur;
}
