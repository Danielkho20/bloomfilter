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

void insert(struct bf *s, struct bd elem){
	uint32_t k = s->k;
	uint32_t key = s->key;
	uint32_t *Tk = malloc(sizeof(uint32_t)*k);
	for(int i=0;i<k;i++){
		size_t h = (key+i)%4294967296;//2^32
		Tk[i] = hash_tp8l3mi(h,elem.sz,elem.dat)%s->m;//La fonction de hachage doit renvoyer des valeurs dans [0;m-1]
	}
	for(uint64_t i=0;i<k;i++){
		uint32_t j = Tk[i]/64;
		s->bits[j] = s->bits[j]|pow64(2,Tk[i]%64); //On utilise l'opérateur ou bit à bit pour ajouter un 1 au bon endroit
	}
	free(Tk);
}

bool prbly_in_set(struct bf *s, struct bd elem){
	uint32_t k = s->k;
	uint32_t key = s->key;
	size_t m = s->m;
	size_t bsize = 0;
	if(m%64==0){
		bsize = m/64;
	}
	else{
		bsize = (m/64)+1;
	}
	uint64_t *Tk = malloc(sizeof(uint64_t)*k);
	for(int i=0;i<k;i++){
		size_t h = (key+i)%4294967296;//2^32
		Tk[i] = hash_tp8l3mi(h,elem.sz,elem.dat)%s->m;
	}
	//On créé un tableau B que l'on va comparer avec le tableau bits pour tester si l'élément est présent
	uint64_t *B = malloc(sizeof(uint64_t)*bsize); 
	for(int i=0;i<bsize;i++){
		B[i] = 0;
	}
	for(uint32_t i=0;i<k;i++){
		uint32_t j = Tk[i]/64;
		B[j] = B[j]|pow64(2,Tk[i]%64);//On insère les éléments dans le tableau B
	}
	bool present = true;
	for(int i=0;i<ceil(s->m/64);i++){
		if ((B[i]&s->bits[i])!=B[i]){//On compare les tableaux
			present = false;
		}
	}
	free(Tk);
	free(B);
	return present;
}


bool test_insert(size_t m ,uint64_t n){
	srandom(time(NULL));
	uint32_t key = random()%4294967295;
	struct bf *s = create_bf(key,m,n);
	uint64_t *Telemi = malloc(sizeof(uint64_t)*n);
	for(uint64_t i=0;i<n;i++){//On créé n éléments que l'on insère
		Telemi[i] = random();	
		struct bd elem = {sizeof(uint64_t),&Telemi[i]};
		insert(s,elem);
	}
	bool test = true;
	for(uint64_t i=0;i<n;i++){//On appelle la fonction prbly_in_set sur nos éléments
		struct bd elem = {sizeof(uint64_t),&Telemi[i]};
		test = prbly_in_set(s,elem);
		if(test != true){
			free(Telemi);
			delete_bf(&s);
			return false;
		}		
	}
	free(Telemi);
	delete_bf(&s);
	return true;
}

uint64_t test_fp(size_t m, uint64_t n, uint64_t nfpt){//Comme pour test insert, on crée n éléments et on appelle ensuite prbly_in_set
	srandom(time(NULL));
	uint32_t key = random()%4294967295;
	struct bf *s = create_bf(key,m,n);
	uint64_t *Telemi = malloc(sizeof(uint64_t)*n);
	for(uint64_t i=0;i<n;i++){
		Telemi[i] = random();	
		struct bd elem = {sizeof(uint64_t),&Telemi[i]};
		insert(s,elem);
	}
	uint64_t nb = 0;
	for(uint64_t i=0;i<nfpt;i++){
		uint64_t elemi = random();	
		struct bd elem = {sizeof(uint64_t),&elemi};
		//printf("%ld",i);
		bool test = prbly_in_set(s,elem);//contrairement à test_insert, on appelle la fonction sur nfpt autres éléments
		if(test){
			nb ++;
		}
	}
	free(Telemi);
	delete_bf(&s);
	return nb;		
	
	
}

int main(){
	srandom(time(NULL));
	uint32_t key = random(); //Si besoin, on évite d'avoir une valeur trop grande pour la taille de key
	//printf("%d",test_insert(8,4));
	//struct bf *s = create_bf(key,8,4);
	//delete_bf(&s);
	///TESTS Q7,8
	/*printf("%lu\n",test_fp(64,64,100000));
	printf("%lu\n",test_fp(1280,1280,100000));
	printf("%lu\n",test_fp(64,64*6,100000));
	printf("\n");
	for(int i = 1;i<41;i++){
		printf("%lu\n",test_fp(64*i,64,64000));
	}
	printf("\n");
	for(int i = 1;i<41;i++){
		printf("%lu\n",test_fp(100*i,100,100000));
	}
	printf("\n");
	for(int i = 1;i<41;i++){
		printf("%lu\n",test_fp(256*i,256,256000));
	}
	printf("\n");
	for(int i = 1;i<41;i++){
		printf("%lu\n",test_fp(1024*i,1024,1024000));
	}*/
	//printf("4294967295\n");
	//printf("%lu",random()*2);

}


//RAND_MAX = 2**31-1




