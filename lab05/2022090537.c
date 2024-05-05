#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST{
	int value;
	struct BST* left;
	struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	Tree root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				if(!findNode(root, key)){
					root = insertNode(root, key);
				}
				else
					fprintf(fout, "insertion error: %d is already in the tree\n", key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key))
					fprintf(fout, "%d is in the tree\n", key);
				else
					fprintf(fout, "finding error: %d is not in the tree\n", key);
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
					fprintf(fout, "delete %d\n", key);
				}
				else{
					fprintf(fout, "deletion error: %d is not in the tree\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL)
						fprintf(fout, "tree is empty");
					else
						printInorder(root);
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}
/*
Insert the value "key" in Tree "root"
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the new node to insert.
return:
	pointer of root
print out:
	"insert key\n"
*/
Tree insertNode(Tree root, int key){
	if(root == NULL){
		root = (Tree) malloc(sizeof(struct BST));
		root->value = key;
		root->left = root->right = NULL;

		fprintf(fout, "insert %d\n", root->value);

	}
	else if(key < root->value){
		root->left = insertNode(root->left, key);
	}
	else if(key > root->value){
		root->right = insertNode(root->right, key);
	}
	return root;
}
/*
Delete the value key in Tree root
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the new node to be deleted.
return :
	pointer of root
*/
Tree deleteNode(Tree root, int key){
	Tree tmp;
	if (key < root->value){ // 왼쪽으로 탐색
		root->left = deleteNode(root->left, key);
	}
	else if(key > root->value){ // 오른쪽으로 탐색
		root->right = deleteNode(root->right, key); 
	}
	else if(root->left && root->right){ // key가 현재 노드의 value와 일치하고 양쪽에 자식 노드가 존재하는 경우
		tmp = root;

		while(root->left != NULL){
			tmp = tmp -> left;
		}

		root -> value = tmp -> value;
		root -> right = deleteNode(root->right, root->value);
	}
	else{ //key == 현재 노드의 value && 자식 노드가 없거나 한 쪽만 존재하는 경우
		tmp = root;
		if(root->left == NULL){
			root = root -> right;
		}
		else{
			root = root -> left;
		}
		free(tmp);
	}
	return root;
}
/*
Find the value key in Tree root
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the node trying to find.
return :
	1, key is found
	0, key is not found
*/
int findNode(Tree root, int key){
	if(root == NULL) return 0;

	if(key < root->value) return findNode(root->left, key);
	if(key > root->value) return findNode(root->right, key);

	return 1;
}
/*
In Order Traversal
Tree root : A pointer to the root node of the binary search tree.
print out:
	"root->value "
*/
void printInorder(Tree root){
	if(root == NULL){

		return;
	}

	printInorder(root -> left);
	fprintf(fout, "%d ", root->value);
	printInorder(root->right);
	
}
/*
Delete Tree
free allocated memory
Tree root : A pointer to the root node of the binary search tree.
*/
void deleteTree(Tree root){
	if (root == NULL){
		return;
	}

	deleteTree(root->left);
	deleteTree(root->right);

	free(root);
}
