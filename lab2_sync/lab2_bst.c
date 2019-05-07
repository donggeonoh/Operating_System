/*
 *	Operating System Lab
 *	    Lab2 (Synchronization)
 *           Student id :        32152661        32154022 
 *           Student name :      Oh Donggeon     Jang Hyeonung
 *
 *   lab2_bst.c :
 *       - thread-safe bst code.
 *       - coarse-grained, fine-grained lock code
 *
 *   Implement thread-safe bst for coarse-grained version and fine-grained version.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"

pthread_mutex_t mutex;

//A function that returns the address of a node that has a key value.
/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree *tree) {
	// You need to implement lab2_node_print_inorder function.
	print_inorder(tree->root);
}

int print_inorder(lab2_node* root) {
	if(root != NULL) {
		print_inorder(root->left);
		print_inorder(root->right);
	}
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree* lab2_tree_create() {
	// You need to implement lab2_tree_create function.
	lab2_tree* my_tree = (lab2_tree *) malloc(sizeof(lab2_tree));
	my_tree->root = NULL;
	return my_tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node* lab2_node_create(int key) {
	// You need to implement lab2_node_create function.
	lab2_node* node = (lab2_node *) malloc(sizeof(lab2_node));
	pthread_mutex_init(&node->mutex, NULL);
	node->left = NULL;
	node->right = NULL;
	node->key = key;
	return node;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node) {
	// You need to implement lab2_node_insert function.
	
	lab2_node* p = tree->root;	//child
	lab2_node* q = NULL;		//parent
	
	while(p) {	//repeat until node find the position of the new node
		q = p;
		if(new_node->key == p->key) {
			return -1;
		}

		if(new_node->key < p->key) p = p->left;
		else p = p->right;
	}
	
	if(tree->root == NULL) tree->root = new_node; //root is null
	else if(new_node->key < q->key) q->left = new_node;
	else q->right = new_node;

	return 0;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node) {
	// You need to implement lab2_node_insert_fg function.
       
        lab2_node* p = tree->root;
        lab2_node* q = NULL;

        while(p) {
                q = p;

                if(new_node->key == p->key) {
                        return -1;
                }
                if(new_node->key < p->key) {
			p = p->left;
		}
                else {
			p = p->right;
		}
        }
	
	pthread_mutex_lock(&new_node->mutex);
        if(tree->root == NULL) {
		tree->root = new_node;
		pthread_mutex_unlock(&new_node->mutex);
	}
        else if(new_node->key < q->key) {
		q->left = new_node;
		pthread_mutex_unlock(&new_node->mutex);
	}
        else {
		q->right = new_node;
        	pthread_mutex_unlock(&new_node->mutex);
	}

        return 0;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node) {
	// You need to implement lab2_node_insert_cg function.
        // You need to implement lab2_node_insert function.

	pthread_mutex_lock(&new_node->mutex);
	
	lab2_node* p = tree->root;
	lab2_node* q = NULL;

        while(p) {
                q = p;
                if(new_node->key == p->key) {
                        return -1;
                }
                if(new_node->key < p->key) p = p->left;
                else p = p->right;
        }

        if(tree->root == NULL) tree->root = new_node;
        else if(new_node->key < q->key) q->left = new_node;
        else q->right = new_node;

	pthread_mutex_unlock(&new_node->mutex);
        
	return 0;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key) {
	// You need to implement lab2_node_remove function.

	lab2_node* p = tree->root;	//parent
	lab2_node* q = tree->root;	//child
	lab2_node* temp = NULL;		

	while(q != NULL) {		//find the same value of key
		if(key == q->key) {
			break;
		}
		p = q;

		if(q->key > key) {
			q = q->left;
		}
		else {
			q = q->right;
		}
	}
	if(q != NULL) {			
		if(q->left != NULL && q->right != NULL) {//if q have both child
			temp = q;
			temp = q->right;

			if(temp->left != NULL) {//if q->left is not null
				while(temp->left != NULL) {//find the node
					temp = temp->left; //the smallest value
				}

				q->key = temp->key;
			}
			else {			//q->left is null
				q->key = temp->key;
				q->right = temp->right;
			}
		}
		else if(q->left != NULL) {	//if q have left child
			lab2_node* temp = q;
			if(temp == tree->root) {
				tree->root = q->left;
			}
			else {
				p->left = q->left;
			}
		}
		else if(q->right != NULL) {	//if q have right child
			lab2_node* temp = q;
			if(temp == tree->root) {
				tree->root = q->right;
			}
			else {
				p->right = q->right;
			}
		}
		else {				//if q have not child
			if(q == tree->root) {
				tree->root = NULL;
			}
			else {
				q = NULL;
			}
		}
	}
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key) {
	// You need to implement lab2_node_remove_fg function.

	lab2_node* p = tree->root;	//parent
	lab2_node* q = tree->root;	//child

	while(q != NULL) {
		if(key == q->key) {
			break;
		}
		p = q;

		if(q->key > key) {
			q = q->left;
		}
		else {
			q = q->right;
		}
	}
	if(q != NULL) {

		if(q->left != NULL && q->right != NULL) {
			lab2_node* temp = q;
			temp = q->right;

			if(temp->left != NULL) {
				while(temp->left != NULL) {
					temp = temp->left;
				}
				q->key = temp->key;
			}
			else {
				q->key = temp->key;
				q->right = temp->right;
			}
		}
		else if(q->left != NULL) {
			lab2_node* temp = q;
			
			if(temp == tree->root) {
				pthread_mutex_lock(&mutex);
				tree->root = q->left;
				pthread_mutex_unlock(&mutex);
			}
			else {
				pthread_mutex_lock(&mutex);
				p->left = q->left;
				pthread_mutex_unlock(&mutex);
			}
		}
		else if(q->right != NULL) {
			lab2_node* temp = q;
			
			if(temp == tree->root) {
				pthread_mutex_lock(&mutex);
				tree->root = q->right;
				pthread_mutex_unlock(&mutex);
			}
			else {
				pthread_mutex_lock(&mutex);
				p->right = q->right;
				pthread_mutex_unlock(&mutex);
			}
		}
		else {
			if(q == tree->root) {
				pthread_mutex_lock(&mutex);
				tree->root = NULL;
				pthread_mutex_unlock(&mutex);
			}
			else {
				pthread_mutex_lock(&mutex);
				q = NULL;
				pthread_mutex_unlock(&mutex);
			}
		}
	}
}


/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key) {
	// You need to implement lab2_node_remove_cg function.

	pthread_mutex_lock(&mutex);

	lab2_node* p = tree->root;	//parent
	lab2_node* q = tree->root;	//child

	while(q != NULL) {
		if(key == q->key) {
			break;
		}
		p = q;

		if(q->key > key) {
			q = q->left;
		}
		else {
			q = q->right;
		}
	}
	if(q != NULL) {
		if(q->left != NULL && q->right != NULL) {
			lab2_node* temp = q;

			temp = q->right;

			if(temp->left != NULL) {
				while(temp->left != NULL) {
					temp = temp->left;
				}

				q->key = temp->key;
			}
			else {
				q->key = temp->key;
				q->right = temp->right;
			}
		}
		else if(q->left != NULL) {
			lab2_node* temp = q;
			if(temp == tree->root) {
				tree->root = q->left;
			}
			else {
				p->left = q->left;
			}
		}
		else if(q->right != NULL) {
			lab2_node* temp = q;
			if(temp == tree->root) {
				tree->root = q->right;
			}
			else {
				p->right = q->right;
			}
		}
		else {
			if(q == tree->root) {
				tree->root = NULL;
			}
			else {
				q = NULL;
			}
		}
	}
	pthread_mutex_unlock(&mutex);
}

/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree) {
	// You need to implement lab2_tree_delete function.
	free(tree);
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node) {
	// You need to implement lab2_node_delete function.
	free(node);
}
