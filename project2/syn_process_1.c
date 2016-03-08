#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "display.h"

int main()
{
	int i, my_sem, my_sem1, v1,v2,t;

	my_sem = semget(IPC_PRIVATE, 1, 0600);

	struct sembuf up = {0, 1, 0};       
	struct sembuf down = {0, -1, 0};
	if (fork())
	{
		semop(my_sem, &up, 1);      /* UP (); */
		for (i=0;i<10;i++){
			semop(my_sem, &down, 1);      /* down (); */
			display("Hello world\n");
			semop(my_sem, &up, 1);      /* up1 (); */
		}
		wait(NULL);
	}
	else
	{
	for (i=0;i<10;i++){
		semop(my_sem, &down, 1);      /* down1(); */
		display("Kalimera kosme\n");
		semop(my_sem, &up, 1);      /* UP (); */
		}
	}
	semctl(my_sem, 0, IPC_RMID);
return 0;
}