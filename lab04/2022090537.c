#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

#define FROMPARENT 0
#define FROMTHREAD 1

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
	int left_thread; // flag if ptr is thread, 1, if it is a thread or 0, if it points to a child node
	ThreadedPtr left_child;
	ElementType data;
	ThreadedPtr right_child;
	int right_thread; // flag if ptr is thread, 1, if it is a thread or 0, if it points to a child node
}ThreadedTree;

/*
Create a root of new Threaded Tree
Root data should be -1
return:
	the pointer of new Threaded Tree
*/
ThreadedPtr CreateTree(){
	ThreadedPtr node_r = (ThreadedPtr)malloc(sizeof(ThreadedPtr));
	node_r -> data = -1;
	node_r -> left_child = node_r;
	node_r -> left_thread = 0;
	node_r -> right_thread = 1;
	return node_r;
}


/*
Insert the key value "data" in Threaded Tree
root : Pointer to the root node of Threaded Tree
root_idx : Index of the root node of Threaded Tree, means the first node to find the insertion position
data : data key value of the node to be newly inserted,
idx : The index of the node to be newly inserted, 0 means root index
return :
	1, the insertion succeeds
	0, the insertion fails.
*/
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx){
	ThreadedPtr newnode = (ThreadedPtr)malloc(sizeof(ThreadedPtr));
	ThreadedPtr tmp;

	if(root->right_child == NULL){ // tree에 root만 존재할 경우, node A 직접 생성
		root->right_child = newnode;
		
		newnode-> left_thread = 1, newnode-> right_thread =1;
		newnode->data = data;
		
		return 1;
	}

	int arr_idx[8];
	int depth = 0;
	
	while(idx/2 > 0){
		arr_idx[depth++] = idx%2;
		idx = idx/2;
	}
	if(idx/2 == 0) arr_idx[depth] = idx%2;

	ThreadedPtr currentnode = root->right_child;

	for(int i = depth; i >= 0; i--){

		if(arr_idx[i] == 0) {
			
			if(currentnode->left_child == NULL) {
				tmp = currentnode->left_child;
				currentnode->left_child = newnode;
				newnode->left_child = tmp;
				newnode->left_thread = 1;
				newnode->right_child = currentnode;
				newnode->right_thread = 1;
				newnode->data = data;
				currentnode->left_thread = 0, currentnode->right_thread = 0;
				
				return 1;

			}

			currentnode = currentnode->left_child;

		}
		else if(arr_idx[i] == 1) {
			if(currentnode->right_child == NULL) {
				tmp = currentnode->right_child;
				currentnode->right_child = newnode;
				newnode->right_child = tmp;
				newnode->right_thread = 1;
				newnode->left_child = currentnode;
				newnode->left_thread = 1;
				newnode->data = data;

				currentnode->left_thread = 0, currentnode->right_thread = 0;

				return 1;
			}

			currentnode = currentnode->right_child;

		}
	}
	
	if(newnode == NULL) return 0;
	else if(currentnode->left_child == NULL && currentnode->right_child == NULL) return 0;
	
}/*
left child부터 탐색하며 right_child == NULL인 node가 있는지 확인
left child == NULL이면 뒤로 
오른쪽만 스레드인 경우가 있는가 -> 
양쪽 다 스레드인 경우에는 어떻게 해야하지?
처음부터 삽입. 그렇다면 어떻게? left_child, right_chile가 null이면 
*/

/*
In Order Traversal
Implementing in a recursive form is prohibited.
When printing out, the interval between each key is one space
print out:
	"inorder traversal : data1 data2 data3" (When there are 3 data, except root node)

*/
void printInorder(ThreadedPtr root){
	ThreadedPtr current_node = NULL;
	ThreadedPtr tmp_root = root;

	fprintf(fout, "inorder traversal : ");
	
	while(current_node != root){
		current_node = tmp_root -> right_child;
	
		if(!current_node->right_thread){
			while(!current_node->left_thread){
				current_node = current_node->left_child;
			}
			fprintf(fout, "%d ", current_node->data);
			current_node = current_node->right_child;
		}

		while(current_node->right_thread){
			fprintf(fout, "%d ", current_node->data);
			current_node = current_node->right_child;
		}
		if(!current_node->right_thread){
			fprintf(fout, "%d ", current_node->data);
			tmp_root = current_node;
		}
	}
}

/*
Delete the Tree
*/
void DeleteTree(ThreadedPtr root){
	
}

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	ThreadedPtr root = CreateTree();

	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);

	int root_idx=0, idx=0;

	while(++idx <= NUM_NODES){
		ElementType data;
		fscanf(fin, "%d", &data);
		
		if(Insert(root, root_idx, data, idx) == 0){
			fprintf(fout, "Insertion failed!\n");
			return 0;
		}
	}

	printInorder(root);
	//DeleteTree(root);
	
	
	fclose(fin);
	fclose(fout);

	return 0;
}
