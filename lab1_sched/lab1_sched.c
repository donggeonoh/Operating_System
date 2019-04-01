/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 	32152661
*	    Student name : 	Oh Donggeon
*
*   lab1_sched.c :
*       - Lab1 source file.
*       - Must contains scueduler algorithm function'definition.
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

void reset(Queue *queue) {
	queue->size = 0;
	queue->front = NULL;
	queue->rear = NULL;
}

int isEmpty(Queue *queue) {
	if(queue->size == 0) {
		return 1;
	}
	return 0;
}

void push(Queue *queue, int numPs, int st) {
	Node *value = (Node *) malloc(sizeof(Node));
	
	value->numPs = numPs;
	value->st = st;
	value->next = NULL;

	if(isEmpty(queue)) {
		queue->front = value;
		queue->rear = value;
	}
	else {
		queue->rear->next = value;
	}
	queue->rear = value;
	queue->size++;
}

void pop(Queue *queue) {
	if(isEmpty(queue)) {
		return;
	}

	Node *value = queue->front;
	queue->front = value->next;
	free(value);

	queue->size--;
}

int act(Queue *queue) {
	Node *value = queue->front;
	value->st--;

	if(queue->front->st == 0) {
		queue->front = queue->front->next;
		free(value);
		return 1;
	}
	else {
		pop(queue);
	}

	return 0;
}

int* expMem(int *sched, int *temp, int time) {
	temp = (int *) malloc(sizeof(int) * (time + 2));
	
	for(int i = 0; i <= time; i++) {
		temp[i] = sched[i];
	}

	free(sched);
	temp[time] = -1;

	return temp;
}

int* fcfs(int numPs, int at[], int st[]) {
	Queue queue;
	int *sched = (int *) malloc(sizeof(int));
	int time = 0;
	int actPs = numPs;
	int i;

	//initialize
	sched[0] = -1;
	reset(&queue);
	
	while(actPs != 0) {
		//expand sched(result value)
		int* temp;
		sched = expMem(sched, temp, time);
                
		for(i = 0; i < numPs; i++) {    //check and add queue..
                        if(at[i] == time) {
				push(&queue, i, st[i]);
                        }
                }

                if(!isEmpty(&queue)) {             //if queue exist
			sched[time] = queue.front->numPs + 1;
			queue.front->st--;
			if(queue.front->st == 0) {
				pop(&queue);
				actPs--;	//if process is done
			}
                }

                time++;
	}

	return sched;
}

int* rr(int numPs, int at[], int st[]) {
	Queue queue;
        int* sched = (int*) malloc(sizeof(int));
	int actPs = numPs;
	int ts = 1;		//time slice!	
	int tq = 0;		//number of time slice..
        int time = 0;
	int i;

	//initialize
        sched[0] = -1;
	reset(&queue);

	printf("\ninput the time slice : ");
	scanf("%d", &ts);

        while(actPs != 0) {
		int* temp;
                sched = expMem(sched, temp, time); //expand result value

		for(i = 0; i < numPs; i++) {    //check and add queue..
                        if(at[i] == time) {
                                push(&queue, i, st[i]);
                        }
                }
		if(ts == tq) {		//time's up!
			tq = 0;
			push(&queue, queue.front->numPs, queue.front->st);
			pop(&queue);
		}

		if(isEmpty(&queue)) {		//if no exist process
			tq = 0;
			time++;
			continue;
		}
		sched[time] = queue.front->numPs + 1;	//store results
		queue.front->st--;

		if(queue.front->st == 0) {		//job is done
			tq = 0;
			pop(&queue);
			actPs--;
			time++;
			continue;
		}
		tq++;
		time++;
	}

	return sched;
}

int powPow(int base, int exp) {
	if(exp == 0)
		return 1;

	for(int i = 1; i < exp; i++) {
		base *= base;
	}
	return base;
}

int* mlfq(int numPs, int at[], int st[]) {
        int* sched = (int*) malloc(sizeof(int));
        int level, base;		//level = level of queue, base = determine the time slice of each level
	int actPs = numPs;
        int curPs = 0;
	int time = 0;
        int tq = 0;			//the left time quantum of running process.
	int i;

        printf("\ninput the number of level : ");
        scanf("%d", &level);

	printf("\ninput the base : ");
	scanf("%d", &base);
	printf("\n");

	//initialize
	Queue queue[level];
	int length[level];		//time slice for level
	for(i = 0; i < level; i++) {
		reset(&queue[i]);
		length[i] = powPow(base, i);
	}
	sched[0] = -1;

        while(actPs != 0) {
                int* temp;
                sched = expMem(sched, temp, time); //expand result value

                for(i = 0; i < numPs; i++) {    //check and add queue..
                        if(at[i] == time) {
				push(&queue[0], i, st[i]);
				
				/*	experiment code..... ignore it
				if(queue[0].size == 1) {
					for(i = 1; i < level; i++) {
						if(!isEmpty(&queue[i])) {
							break;
						}
					}
					if(i == level) {
						printf("time : %d, numPs : %d\n", time, queue[0].front->numPs);
                                       	        push(&queue[1], queue[0].front->numPs, queue[0].front->st);
                                                pop(&queue[0]);
                                                push(&queue[0], i, st[i]);
					}
				}
				else {
					push(&queue[0], i, st[i]);
				}
				*/
			}
                }
		if(tq == 0) {
			for(i = 0; i < level; i++) {
				if(!isEmpty(&queue[i])) {
					curPs = i;
					tq = length[i];
					break;
				}
			}
		}
		if(tq != 0) {
                        sched[time] = queue[curPs].front->numPs + 1;
                        queue[curPs].front->st--;
			tq--;
                        if(queue[curPs].front->st == 0) {
                                pop(&queue[curPs]);
				curPs = 0;
                                tq = 0;
                                actPs--;
			}
                        else if(tq == 0) {
				/*	experiment code... ignore it
                                for(i = 0; i < level; i++) {
					if(i == curPs) {
						if(queue[i].size != 1) {
							break;
						}
					}
                                        else if(!isEmpty(&queue[i])) {
                                                break;
                                        }
                                }
				*/
			//	if(i != level) {
                	                if(level == curPs + 1) {
        	                                push(&queue[curPs], queue[curPs].front->numPs, queue[curPs].front->st);
	                                        pop(&queue[curPs]);
                               		}
                        	        else {
                	                        push(&queue[curPs + 1], queue[curPs].front->numPs, queue[curPs].front->st);
        	                                pop(&queue[curPs]);
	                                }
			//	}
			}
		}

		time++;


		//ignore this below
		/*
		if(tq == 0) {	//choose the next process
			tq = length[curPs];
			printf("time : %d, time quentum : %d, curPs : %d \n",time, tq, curPs);
		}

		if(tq != 0) {	//execute the process
			sched[time] = queue[curPs].front->numPs + 1;
			queue[curPs].front->st--;
			tq--;
			if(queue[curPs].front->st == 0) {
				pop(&queue[curPs]);
				tq = 0;
				actPs--;

				for(i = curPs; i < level; i++) {
					if(!isEmpty(&queue[i])) {
						break;
					}
				}
				if(i == level) {
					curPs = 0;
				}
				else {
					curPs = i;
				}
			}
			else if(tq == 0) {		//maybe here the problem
				Node value = *queue[curPs].front;
				pop(&queue[curPs]);
				
				for(i = curPs; i < level; i++) {
					if(!isEmpty(&queue[i])) {
						break;
					}
				}
				if(level == curPs + 1) {
					push(&queue[curPs], value.numPs, value.st);
				}
				else {
					push(&queue[curPs + 1], value.numPs, value.st);
				}
				if(i == level) {
					if(level != curPs + 1) {
						curPs++;
					}
				}
				else {
					curPs = i;
				}
			}
		}
		time++;
		*/
	}
	return sched; 
}

/*
int* lottery(int numPs, int at[], int st[]) {
	int* sched;

	return sched;
}
*/
