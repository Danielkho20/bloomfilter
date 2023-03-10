#include <stdio.h>
#include <stdint.h>
#include <math.h> //Pensez au -lm à la compilation
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//#define RAND_MAX 18446744073709551616

struct bf{
	uint32_t key; // main hf key
	size_t m; // bitsize
	uint32_t k; // #hf
	uint64_t bits[]; // “s”
};

struct bd{
	size_t sz;
	void *dat;
};

void affiche(size_t Tlen, const uint32_t T[Tlen]){ //permet d'afficher un tableau
	printf("{");
	for(int i=0;i<Tlen-1;i++)
		printf("%u,",T[i]);
	printf("%u}",T[Tlen-1]);
}

uint64_t pow64(uint64_t a, uint64_t b){//Permet de calculer la puissance(la fonction de base en C utilise des doubles)
	uint64_t a1 = a;
	for(int i =1;i<b;i++)
		a = a*a1;
	return a;
}

uint64_t hash_tp8l3mi(uint32_t k, size_t buflen, uint8_t buf[buflen]){
	uint64_t acc = 0;
	for (size_t i = 0; i < buflen; i++){
		acc ^= buf[i];
		acc += (acc << 28) ^ (acc << 34);
		acc *= (k | 1);
	}
	return acc;
}

struct bf *create_bf(uint32_t key, size_t m, uint64_t n){
	uint32_t k = ceil(log(2)*m/n);//On peut changer k ici pour tester la question 8
	size_t bsize = 0;
	if(m%64==0){
		bsize = m/64;
	}
	else{
		bsize = (m/64)+1;
	}
	struct bf *s = malloc(sizeof(struct bf) + bsize*sizeof(uint64_t));//on alloue l'espace nécéssaire pour la struct bf et le tableau bits
	s->key = key;//Accès à key par A->key
	s->m = m;
	s->k = k;
	for(int i = 0;i<bsize;i++){
		s->bits[i] = 0;//On initialise le tableau à 0
	}
	return s;	
};

void delete_bf(struct bf **s){
	free(*s);
	*s = NULL;
}





int main(){
	srandom(time(NULL));
	uint32_t key = random(); //Si besoin, on évite d'avoir une valeur trop grande pour la taille de key
}


//RAND_MAX = 2**31-1




