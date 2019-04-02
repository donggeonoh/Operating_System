/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id :	32152661
*	    Student name : 	Oh Donggeon
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm test code.
*
*/

#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <pthread.h>
#include <asm/unistd.h>

#include "lab1_sched_types.h"

/*
 * you need to implement scheduler simlator test code.
 *
 */

void msg() {
	puts("1. FCFS 2. RR 3. Feedback(q=1) 4. Feedback(q=2^i) 5. Lottary");
        puts("Please enter the number...");
}

void inputPs(int numPs, int at[], int st[]) {     //input value of process

        printf("\n");
        for(int i = 0; i < numPs; i++) {
                printf("input the arrival time & service time of process %c : ", 65 + i);
                scanf("%d %d", &at[i], &st[i]);
        }
}

void print(int* sched, int numPs) {
	int i, j;
	for(i = 0; sched[i] != 0; i++);		//get the size of result
	int size = i;				//result size

	//print
	printf("\n");
	printf("    ");
	for(i = 0; i < size; i++) {
		printf("%d ", i);
	}
	printf("\n");
	for(i = 1; i <= numPs; i++) {
		printf("%c : ", 64 + i);
		for(j = 0; j < size; j++) {
			if(i == sched[j]) {
				if(j / 10 > 0) {	//if time is over 10
					printf("==|");
					continue;
				}
				printf("=|");
			}
			else {
				if(j / 10 > 0) {	//if time is over 10
					printf("  |");
					continue;
				}
				printf(" |");
			}
		}
		printf("\n");
	}
	printf("\n");
}

int schedule(char* ch) {
	char input = ch[0];
	int numPs = 1;
	int* sched;

	switch(input) {
		case '1':
		case '2':
		case '3':
		case '4':
			printf("input the number of process : ");
			scanf("%d", &numPs);
			break;
		default:
			msg();
			free(sched);
			return 0;
	}
	int at[numPs];
	int st[numPs];
	
	inputPs(numPs, at, st);

	switch(input) {
		case '1':
			sched = fcfs(numPs, at, st);
			print(sched, numPs);
			break;
		case '2':
			sched = rr(numPs, at, st);
			print(sched, numPs);
			break;
		case '3':
			sched = mlfq(numPs, at, st);
			print(sched, numPs);
			break;
		case '4':
			sched = lottery(numPs, at, st);
			print(sched, numPs);
			break;
		default :
			msg();
	}
	
	free(sched);
	return 0;
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		msg();
		return 0;
	}
	schedule(argv[1]);

	return 0;
}
