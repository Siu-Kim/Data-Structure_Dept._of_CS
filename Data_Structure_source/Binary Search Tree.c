#include <stdio.h>
#include <stdlib.h>

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
Tree InorderSucc(Tree root);

int tmp = 1; //printInorder 에서 쓰이는 변수임. 첫번째 출력을 공백 없이 진행하기 위해서 도입
void main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    char cv;
    int key;

    Tree root = NULL;

    while(EOF!=fscanf(fin, "%c", &cv)){
        switch(cv){
            case 'i':
                fscanf(fin, "%d", &key);
                root = insertNode(root, key);
                if(root == NULL){
                    return; //insertNode를 진행 한 후 메모리 용량이 없는 경우에서 NULL값을 반환하므로
                            //root에 NULL이 들어가면 main함수 종료
                }
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if(findNode(root, key)){
                    fprintf(fout, "%d is in the tree\n", key);
                }else{
                    fprintf(fout, "finding error: %d is not in the tree\n", key);
                }
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                if(findNode(root, key)){
                    root = deleteNode(root, key);
                    fprintf(fout, "delete %d\n", key);
                }else{
                    fprintf(fout, "deletion error: %d is not in the tree\n", key);
                }
                break;
            case 'p':
                fscanf(fin, "%c", &cv);
                if(cv == 'i'){
                    if(root == NULL){
                        fprintf(fout, "tree is empty");
                    }else{
                        printInorder(root);
                        tmp = 1; //pi를 두번 이상 진행하는 경우를 위해 tmp를 다시 1로 만듬
                    }
                }
                fprintf(fout, "\n");
                break;
        }
    }
    deleteTree(root);
}

Tree insertNode(Tree root, int key){
    if(root == NULL){
        root = (Tree)malloc(sizeof(struct BST));
        if(root == NULL){
            fprintf(fout, "out of space!");
            return NULL; //메모리 용량이 없을 경우 NULL 반환
        }
        root->value = key;
        root->left = root->right = NULL;
        fprintf(fout, "insert %d\n", key);
    }else if(key < root->value){
        root->left = insertNode(root->left, key);
    }else if(key > root->value){
        root->right = insertNode(root->right, key);
    }else{ //이미 있는 값을 insert할 경우
        fprintf(fout, "insertion error: %d is already in the tree\n", key);
    }
    return root; //맨 꼭대기(루트) 노드를 반환
}
Tree deleteNode(Tree root, int key){
    Tree tmp;
    if(key < root->value){
        root->left = deleteNode(root->left, key);
    }else if(key > root->value){
        root->right = deleteNode(root->right, key);
    }else if(root->left != NULL && root->right != NULL){ //child가 2인 경우
        tmp = InorderSucc(root); //오른쪽 subtree의 가장 작은 값으로 바꾼 후
        root->value = tmp->value;
        root->right = deleteNode(root->right, root->value); //그 값을 찾아 삭제
    }else{ //child가 0 또는 1인 경우
        tmp = root;
        if(root->left == NULL){ //왼쪽 자식이 없는 경우
            root = root->right;
        }else{ //오른쪽 자식이 없는 경우
            root = root->left;
        }
        free(tmp);
    }
    return root;
}
int findNode(Tree root, int key){
    if(root == NULL){
        return 0; //key에 대한 노드가 없다면 0을 반환
    }
    if(key < root->value){
        findNode(root->left, key);
    }else if(key > root->value){
        findNode(root->right, key);
    }else{
        return 1; //key에 대한 노드가 있다면 1을 반환
    }
}
void printInorder(Tree root){
    if(root == NULL){
        return;
    }
    printInorder(root->left);
    if(tmp == 1){ //첫번째 출력만 해당됨
        fprintf(fout, "%d", root->value);
        tmp--;
    }else{
        fprintf(fout, " %d", root->value); 
    }
    printInorder(root->right);
}
void deleteTree(Tree root){
     if(root == NULL){
         return;
     }
     deleteTree(root->left);
     deleteTree(root->right);
     free(root);
}
Tree InorderSucc(Tree root){ //오른쪽 subTree의 가장 왼쪽 노드를 반환
    root = root->right;
    while(1){
        if(root->left == NULL){
            break;
        }
        root = root->left;
    }
    return root;
}

