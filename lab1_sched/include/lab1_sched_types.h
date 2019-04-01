/*
*	DKU Operating System Lab
*	    Lab1 (Scheduler Algorithm Simulator)
*	    Student id : 	32152661
*	    Student name : 	Oh Donggeon
*
*   lab1_sched_types.h :
*       - lab1 header file.
*       - must contains scueduler algorithm function's declations.
*
*/

#ifndef _LAB1_HEADER_H
#define _LAB1_HEADER_H


/*
 * You need to Declare functions in  here
 */

typedef struct Node {
	int numPs;	//process number;
	int st;		//service time

	struct Node *next;
}Node;

typedef struct Queue {
	int size;

	struct Node *front;
	struct Node *rear;
}Queue;

int isEmpty(Queue *queue);
void push(Queue *queue, int numPs, int st);
void pop(Queue *queue);
int act(Queue *queue);
int* expMem(int* sched, int* temp, int time);
int powPow(int base, int exp);

int* fcfs(int numPs, int at[], int st[]);
int* rr(int numPs, int at[], int st[]);
int* mlfq(int numPs, int at[], int st[]);
int* lottery(int numPs, int at[], int st[]);

#endif /* LAB1_HEADER_H*/
