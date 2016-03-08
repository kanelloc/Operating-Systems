#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "display.h"
#include <pthread.h>

pthread_mutex_t mutex;
pthread_t thread1;
pthread_t thread2;
pthread_cond_t cond_var;
int predicate1 = 0;
int predicate2 = 0;

void * displaymsg1(){
	int i;
	predicate1 = 1; //prealocation to excpell the first while to display ab.
	for (i=0;i<10;i++){
		pthread_mutex_lock(&mutex); //W8 for the next event to display.
		while (predicate1 == 0){
			pthread_cond_wait(&cond_var,&mutex);
		}
		display("ab");
		predicate1 = 0;
		predicate2 = 1;

		pthread_cond_signal(&cond_var); //Jump to the next display thread.
		pthread_mutex_unlock(&mutex);
	}
}

void * displaymsg2(){
	int i;
	for (i=0;i<10;i++){
		pthread_mutex_lock(&mutex);
		while (predicate2 == 0){
			pthread_cond_wait(&cond_var,&mutex); //w8 to display the msg and then replay the 1st thread.
		}
		display("cd\n");
		predicate2 = 0;
		predicate1 = 1;
		pthread_cond_signal(&cond_var);
		pthread_mutex_unlock(&mutex);
	}
}


int main(){
	pthread_mutex_init(&mutex, 0);
	pthread_cond_init(&cond_var, NULL);
	pthread_create(&thread1,0, displaymsg1,NULL); //Create the first thread
	pthread_create(&thread2,0, displaymsg2,NULL); //Creat the second thread.
	pthread_join(thread1, NULL); //w8 the thread1 to execute.
	pthread_join(thread2, NULL); //w8 the thread2 to execute.
	pthread_mutex_destroy(&mutex); //Destroy the mutex
}