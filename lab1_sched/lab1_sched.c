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

int* expMem(int *sched, int *temp, int time) {
	temp = (int *) malloc(sizeof(int) * (time + 2));
	
	for(int i = 0; i <= time; i++) {
		temp[i] = sched[i];
	}
	free(sched);
	temp[time] = -1;
	temp[time + 1] = 0;

	return temp;
}

int* fcfs(int numPs, int at[], int st[]) {
	Queue queue;
	int *sched = (int *) malloc(sizeof(int));	//result value
	int time = 0;
	int actPs = numPs;	//remain running process in total process.
	int i;

	//initialize
	sched[0] = -1;
	reset(&queue);
	
	while(actPs != 0) {	//All process is done -> escape!
		int* temp;
		sched = expMem(sched, temp, time);	//expand result value
                
		for(i = 0; i < numPs; i++) {    //check and add queue..
                        if(at[i] == time) {
				push(&queue, i, st[i]);
                        }
                }

                if(!isEmpty(&queue)) {             //if queue exist
			sched[time] = queue.front->numPs + 1;	//input the execute process (if process 0 execute -> put in 1(A), 1 -> 2(B))
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
        int level, base;		//level = level of queue, base = determine the time slice for each level
	int actPs = numPs;
        int curPs = 0;
	int time = 0;
        int tq = 0;			//the left time quantum of running process.
	int i;

        printf("\ninput the number of level : ");
        scanf("%d", &level);

	printf("\ninput the base : ");
	scanf("%d", &base);

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
                sched = expMem(sched, temp, time); //expand result value (return value)

                for(i = 0; i < numPs; i++) {    //check and add queue..
                        if(at[i] == time) {
				int num = 0;
				int index = -1;
				for(int j = 0; j < level; j++) {	//count the queue
					if(!isEmpty(&queue[j])) {
						num += queue[j].size;
						index = j;
					}
				}
				if(num == 1) {				//if queue is alone
					if(index == 0) {		//priority not going down
						push(&queue[0], i, st[i]);
						push(&queue[0], queue[0].front->numPs, queue[0].front->st);
						pop(&queue[0]);
					}
					else {
						push(&queue[0], i, st[i]);
					}
				}
				else {
					push(&queue[0], i, st[i]);
				}
			}
                }
		if(tq == 0) {	//times up!!!
			for(i = 0; i < level; i++) {
				if(!isEmpty(&queue[i])) {
					curPs = i;
					tq = length[i];
					break;
				}
			}
		}
		if(tq != 0) {	//excute queue
                        sched[time] = queue[curPs].front->numPs + 1;
                        queue[curPs].front->st--;
			tq--;
                        if(queue[curPs].front->st == 0) { //queue is done
                                pop(&queue[curPs]);
				curPs = 0;
                                tq = 0;
                                actPs--;
			}
                        else if(tq == 0) {	//when the queue time's up
                                for(i = 0; i < level; i++) {	//if queue is alone
					if(i == curPs) {
						if(queue[i].size != 1) {
							break;
						}
					}
                                        else if(!isEmpty(&queue[i])) {
                                                break;
                                        }
                                }
				if(i != level) {	//priority not going down
                	                if(level == curPs + 1) {
        	                                push(&queue[curPs], queue[curPs].front->numPs, queue[curPs].front->st);
	                                        pop(&queue[curPs]);
                               		}
                        	        else {
                	                        push(&queue[curPs + 1], queue[curPs].front->numPs, queue[curPs].front->st);
        	                                pop(&queue[curPs]);
	                                }
				}
			}
		}
		time++;
	}
	return sched; 
}

int* lottery(int numPs, int at[], int st[]) {
        int* sched = (int*) malloc(sizeof(int));
	int* totalTicket;	//determine = total ratio of executing process * mag
	int mag = 10;		//number of tickets per ratio (ratio = 2, tickets = 20 / ratio = 3, tickets = 30)
	int ratio[numPs];
	int ticket[numPs];
	int curPs[numPs];	//current activating process
	int sumRatio = 0;	//total ratio of executing process
	int draw = 0;		//index of totalticket
        int actPs = numPs;
	int times = 0;
        int i;

	for(i = 0; i < numPs; i++) {
		printf("input the ratio of process %c : ", 65 + i);
		scanf("%d", &ratio[i]);

		ticket[i] = 0;
		curPs[i] = 0;
	}
        
	//initialize
        srand((int)time(NULL));
	sched[0] = -1;

        while(actPs != 0) {
		int* temp;
		sched = expMem(sched, temp, times);

                for(i = 0; i < numPs; i++) {    //check and add process..
                        if(at[i] == times) {
				curPs[i] = 1;
			}
                }

		for(i = 0; i < numPs; i++) {	//give a ticket and add the ratio of executing process
			if(curPs[i] == 1) {
				ticket[i] = ratio[i] * mag;
				sumRatio += ratio[i];
			}
		}
		int size = sumRatio * mag;	//number of total ticket
		int index = 0;

		totalTicket = (int *) malloc(sizeof(int) * size);
		printf("size : %d\n", size);	
		while(size) {			//put each ticket into the total ticket
			for(i = 0; i < numPs; i++) {
				if(ticket[i] != 0) { 
					for(int j = 0; j < ratio[i]; j++) {
						totalTicket[index] = i;
						printf("t : %d i : %d\n", totalTicket[index], index);
						index++;
						ticket[i]--;
						size--;
					}
				}
			}
		}
		printf("\n");
		size = sumRatio * mag;
		
		if(size) {	//draw for winners!!!
			draw = rand() % size;
			draw = totalTicket[draw];
			sched[times] = draw + 1;
			st[draw]--;

			if(st[draw] == 0) {
				curPs[draw] = 0;
				actPs--;
			}
		}
		free(totalTicket);
		sumRatio = 0;
		times++;
	}

	return sched;
}
