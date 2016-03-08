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

void * displaymsg1(){
	int i;
	//pthread_mutex_unlock(&mutex);
	for (i=0;i<10;i++){
		pthread_mutex_lock(&mutex); //loc the disp until the unlock.
		display("Hello world\n");
		pthread_mutex_unlock(&mutex);
	}
}

void * displaymsg2(){
	int i;
	for (i=0;i<10;i++){
		pthread_mutex_lock(&mutex); //loc the disp until the unlock.
		display("Kalimera kosme\n");
		pthread_mutex_unlock(&mutex);
	}
}

int main(){
	pthread_mutex_init(&mutex, 0);
	pthread_create(&thread1,0, displaymsg1,NULL); //Create the first thread to disp1.
	pthread_create(&thread2,0, displaymsg2,NULL); //Create the second thread to disp2.
	pthread_join(thread1, NULL); //w8 the first thread to end.
	pthread_join(thread2, NULL); //w8 the second thread to end.
	pthread_mutex_destroy(&mutex); //Destroy the mutex.
}

