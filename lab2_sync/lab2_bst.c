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
void print_inorder(lab2_node* root) {
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
				lab2_node_delete(temp);
			}
			else {
				q->key = temp->key;
				q->right = temp->right;
				lab2_node_delete(temp);
			}
		}
		else if(q->left != NULL) {
			lab2_node* temp = q;
			if(temp == tree->root) {
				tree->root = q->left;
				lab2_node_delete(temp);
			}
			else {
				p->left = q->left;
				lab2_node_delete(temp);
			}
		}
		else if(q->right != NULL) {
			lab2_node* temp = q;
			if(temp == tree->root) {
				tree->root = q->right;
				lab2_node_delete(temp);
			}
			else {
				p->right = q->right;
				lab2_node_delete(temp);
			}
		}
		else {
                        if(q == tree->root) {
				lab2_node_delete(tree->root);
				tree->root = NULL;
			}
                        else {
                                lab2_node_delete(q);
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
