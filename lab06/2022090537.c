#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
	ElementType element;
	AVLTree left, right;
	int height;
}AVLNode;

int FindNode(AVLTree T, int value);
int max(int a, int b);
int Height(Position P);
void PrintPreorder(AVLTree T);
void DeleteTree(AVLTree T);
AVLTree Insert(ElementType x, AVLTree T);
AVLTree Delete(ElementType x, AVLTree T);
Position SingleRotateWithLeft(Position P);
Position SingleRotateWithRight(Position P);
Position DoubleRotateWithLeft(Position P);
Position DoubleRotateWithRight(Position P);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLTree Tree = NULL;
	char cv;
	int key;

	int i=0;

	while(!feof(fin)){
		printf("%d: ", ++i);
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				if(FindNode(Tree, key)){
					Tree = Delete(key, Tree);
				}
				else{
					fprintf(fout, "deletion error : %d is not int the tree!\n", key);
				}
				break;
				
		}
		PrintPreorder(Tree);
		fprintf(fout, "\n");
		printf("\n");
	}

//	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);
	return 0;
}


/*
return:
	bigger variable
*/
int max(int a, int b){
	if(a >= b){
		return a;
	}
	else return b;
}

/*
return:
	-1
	the height of current node  
*/
int Height(Position P){
	if(P == NULL) return -1;
	return P->height;
}

/*
Insert the value X in AVLTree T
return:
	pointer of root
print out:
	"insertion error : X is already in the tree!\n" , X is already in T
*/
AVLTree Insert(ElementType X, AVLTree T){
	if(T == NULL){
		T = malloc(sizeof(AVLNode));
		T->element = X;
		T->height = 0;
		T->left = T->right = NULL;
	}
	else if(X < T->element){
		T->left = Insert(X, T->left);
		if(Height(T->left) - Height(T->right) >= 2){
			if(X < T->left->element){
				T = SingleRotateWithLeft(T);
			}
			else{
				T = DoubleRotateWithLeft(T);
			}
		}
	}
	else if(X > T->element){
		T->right = Insert(X, T->right);
		if(Height(T->right) - Height(T->left) >= 2){
			if(X > T->right->element){
				T = SingleRotateWithRight(T);
			}
			else{
				T = DoubleRotateWithRight(T);
			}
		}
	}
	else{
		fprintf(fout, "insertion error : %d is already in the tree!\n", X);
	}
	T->height = max(Height(T->left), Height(T->right)) + 1;
	return T; //recursion의 최종 과정에서 루트 노드를 return
}
/*
recursive하게 BST의 조건을 만족하도록 insert

left subtree의 left child 혹은 right subtree의 right child로 insert할 경우,
SingleRotationLeft/Right(T)
left subtree의 right child 혹은 right subtree의 left child로 insert할 경우,
DoubleRotationLeft/Right(T) 
*/

/*
Delete the value X in AVLTree T
return:
	pointer of root
print out:
	"deletion error : X is not in the tree!\n", X is not in T
*/
AVLTree Delete(ElementType X, AVLTree T){
	AVLTree tmp;
	if(X < T->element){ // delete 하는 값이 왼쪽 서브트리에 존재
		T->left = Delete(X, T->left);
		if(Height(T->left) - Height(T->right) >= 2){
			if(X < T->left->element){ //Left subtree의 left child
				T = SingleRotateWithLeft(T);
			}
			else{ //Left child의 right child
				T = DoubleRotateWithLeft(T);
			}
		}
	}
	else if(X > T->element){ // delete 하는 값이 오른쪽 서브트리에 존재
		T->right = Delete(X, T->right);
		if(Height(T->right) - Height(T->left) >= 2){
			if(X > T->right->element){
				T = SingleRotateWithRight(T);
			}	
			else{
				T = DoubleRotateWithRight(T);
			}
		}
	}
	else if(T->left != NULL && T->right != NULL){//양 쪽 child가 모두 존재 
		tmp = T->right;
		while(1){ // 오른쪽 subtree의 가장 왼쪽 node 탐색(오른쪽 subtree 중 가장 작은 값)
			if(tmp->left == NULL){
				break;
			}
			tmp = tmp->left;
		}//오른쪽 subtree의 가장 작은 값을 찾아 바꾸기
		
		T->element = tmp->element;
		T->right = Delete(T->element, T->right);
	}
	else{ //양 쪽 child가 모두 없거나 한 쪽만 존재. 자식 node가 있다면 자리를 바꾸고 free
		tmp = T;
			if(T->left == NULL){
				T = T->right;
			}
			else{
				T = T->left;
			}
			free(tmp);
	}
	
	if(T != NULL){
		T -> height = max(Height(T->left), Height(T->right)) + 1;
	}
	return T;
}


/*
Pre order Traversal
*/
void PrintPreorder(AVLTree T){
	if(T == NULL) return;

	fprintf(fout, "%d(%d) ", T -> element, Height(T));
	PrintPreorder(T->left);
	PrintPreorder(T->right);
}


/*
Delete Tree
free allocated memory
*/
void DeleteTree(AVLTree T){
	if(T == NULL) return;

	DeleteTree(T->left);
	DeleteTree(T->right);
	free(T);
}

int FindNode(AVLTree T, int value){
	if (T == NULL){
		return 0;
	}
	if(value > T->element){
		FindNode(T->right, value);
	}
	else if(value < T->element){
		FindNode(T->left, value);
	}
	else{
		return 1;
	}
}

/*
Rotation functions for AVLTree
*/
Position SingleRotateWithLeft(Position node){
	Position tmp;
	tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;

	node->height = max(Height(node->left), Height(node->right)) + 1;
	tmp->height = max(Height(tmp->left), node->height) + 1;

	return tmp; /* New root */
}



Position SingleRotateWithRight(Position node){
	Position tmp;
	tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;

	node->height = max(Height(node->right), Height(node->left)) + 1;
	tmp->height = max(Height(tmp->right), node->height) + 1;

	return tmp;
}

Position DoubleRotateWithLeft(Position node){
	node -> left = SingleRotateWithRight(node->left);

	return SingleRotateWithLeft(node);
}
Position DoubleRotateWithRight(Position node){
	node -> right = SingleRotateWithLeft(node->right);

	return SingleRotateWithRight(node);

}