#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int hidrogenio, enxofre, oxigenio;

//semaforos para permitir a producao do atomo
sem_t semHidrogenio;
sem_t semEnxofre; 
sem_t semOxigenio;

void *trh_Hidrogenio();
void *trh_Enxofre();
void *trh_Oxigenio();

int main(){
	int n;
	printf("Informe o número de sequencias: ");
	scanf("%d", &n);

	if(n <= 0)
		return 0;

	//definindo a quantidade que deve ser produzido de cada atomo 
	hidrogenio = 2 * n;
	enxofre = n;
	oxigenio = 4 * n;

	sem_init(&semHidrogenio, 0, 1); // o semaforo de hidrogenio inicia com 1 pois ele deve ser o primeiro a ser produzido
	sem_init(&semEnxofre, 0, 0);
	sem_init(&semOxigenio, 0, 0);

	//criando e iniciando as threads
	pthread_t thdHidrogenio, thdOxigenio, thdEnxofre;

	pthread_create(&thdHidrogenio, 0, (void *) trh_Hidrogenio, NULL);
	pthread_create(&thdEnxofre, 0, (void *) trh_Enxofre, NULL);
	pthread_create(&thdOxigenio, 0, (void *) trh_Oxigenio, NULL);

	pthread_join(thdHidrogenio, 0);
	pthread_join(thdEnxofre, 0);
	pthread_join(thdOxigenio, 0);

	return 0;
}

void *trh_Hidrogenio(){
	for(;;){
		if((hidrogenio % 2) == 0) // caso ainda nao tenha iniciado a producao do par de atomos 
			sem_wait(&semHidrogenio);
		
		printf("H");
		hidrogenio--;

		if((hidrogenio % 2) == 0){ //caso ja tenha sido produzido um par de hidrogenio, libera para a producao de enxofre
			sem_post(&semEnxofre);
			
			if(hidrogenio == 0) //caso ja tenha produzido todos os atomos
				pthread_exit(NULL);
		}
	}
}

void *trh_Enxofre(){
	for(;;){
		sem_wait(&semEnxofre);

		printf("S");
		enxofre--;
		
		sem_post(&semOxigenio); //libera para a producao do oxigenio 

		if(enxofre == 0)
			pthread_exit(NULL); //caso ja tenha produzido todos os atomos
	}
}

void *trh_Oxigenio(){
	for(;;){
		if((oxigenio % 4) == 0) // caso ainda nao tenha iniciado a producao do conjunto de 4 atomos
			sem_wait(&semOxigenio);
		
		printf("O");
		oxigenio--;
		
		if((oxigenio % 4) == 0){ //caso ja tenha sido produzido 4 atomos de oxigenio, libera para a producao de hidrogenio
			printf("\n"); //separacao das moleculas
			sem_post(&semHidrogenio);
		
			if(oxigenio == 0)
				pthread_exit(NULL); //caso ja tenha produzido todos os atomos
		}
	}
}
