#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
    int left_thread;
    ThreadedPtr left_child;
    ElementType data;
    ThreadedPtr right_child;
    int right_thread;
}ThreadedTree;

ThreadedPtr CreateTree();
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx);
ThreadedPtr InorderPred(ThreadedPtr tree); //해당 값에 연결되는 스레드를 찾는 함수
ThreadedPtr InorderSucc(ThreadedPtr tree); //해당 값에 연결되는 스레드를 찾는 함수
void ConnectThread(ThreadedPtr tree); //스레드를 연결해주는 함수
void activateCT(ThreadedPtr root, int root_idx, int idx);//ConnectThread()를 실행해주는 함수
void printInorder(ThreadedPtr root);
void DeleteTree(ThreadedPtr root);


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
            return 0;
        }
    }
    ThreadedPtr pointer = root->right_child; //트리의 왼쪽 아래끝 부분과 오른쪽 아래 끝부분을 root 노드에 연결할 것.
    while(!pointer->left_thread){
        pointer = pointer->left_child;
    }
    pointer->left_child = root; //왼쪽 처리 끝

    pointer = root->right_child;
    while(!pointer->right_thread){
        pointer = pointer->right_child;
    }
    pointer->right_child = root; //오른쪽 처리 끝
   
    idx = 0; //다시 0으로 초기화
    while(++idx <= NUM_NODES){ //스레드를 연결시켜주는 ConnectThread들을 호출해주는 함수
        activateCT(root, root_idx, idx);
    }

    printInorder(root); //출력
    DeleteTree(root); //메모리에서 삭제

    fclose(fin);
    fclose(fout);

    return 0;
}

ThreadedPtr CreateTree() {
    ThreadedPtr tree = NULL;
    tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
    if(tree==NULL){
        fprintf(fout, "Out of Space!\n");
        return NULL;
    }
    tree->data = -1;
    tree->left_thread = 1;
    tree->left_child = tree;
    tree->right_thread = 1;
    tree->right_child = tree;
    return tree;
}
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx){
    ThreadedPtr ptr = root; //root부터 시작해서 idx까지 따라가는 포인터
    ThreadedPtr tree = NULL; //추가할 노드를 가르키는 포인터
    tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
    if(tree==NULL){
        fprintf(fout, "Out of Space!\n");
        return 0;
    }
    tree->data = data; //추가할 노드의 데이터값 저장;
    //idx 의 값을 이진수로 변환하고 1을 오른쪽 0을 왼쪽으로 생각할 것임
    //예를들어 6을 이진수로 변환하면 110인데 root 노드부터 시작해서 오른쪽->오른쪽->왼쪽으로
    //이동하면 6번째 인덱스의 위치임

    //이진수를 담을 배열의 크기를 구하자.
    int tmp = idx;
    int count = 0; //배열의 크기
    while(1){
        tmp /= 2;
        count++;
        if(tmp == 0) break;
    }
    int arr[count]; //이진수를 담을 배열
    for(int i = 0; i < count; i++){ //배열에 이진수를 차례대로 넣음
        arr[count-1-i] = idx%2;
        idx/=2;
    }
    if(count != 1){
        for(int i = root_idx; i < count - 1; i++){ //idx 노드의 부모 노드를 ptr이 가리키게 만들기
            if(arr[i] == 1){ //1인 경우는 오른쪽
                ptr = ptr -> right_child;
            }else if(arr[i] == 0){ //0인 경우는 왼쪽
                ptr = ptr -> left_child;
            }
        }
    }//count가 1인경우는 ptr이 root를 가리킴
    if(arr[count-1] == 1){ //만약 1일 경우 오른쪽에 자식노드를 만들고 0일 경우 왼쪽 노드에 자식을 만듬
        tree->left_thread = 1; //자식을 만들 때 마다 스레드를 1로 만듬
        tree->right_thread = 1;
        ptr->right_child = tree;
        ptr->right_thread = 0; //오른쪽 자식을 추가하므로 오른쪽 스레드는 0
    }else if(arr[count-1] == 0){
        tree->left_thread = 1; //자식을 만들 때 마다 스레드를 모두 1로 만듬
        tree->right_thread = 1;
        ptr->left_child = tree;
        ptr->left_thread = 0; //왼쪽 자식을 추가하므로 왼쪽 스레드는 0
    }
    return 1; //1반환
}
ThreadedPtr InorderPred(ThreadedPtr tree){ //왼쪽 subtree의 가장 오른쪽 값이 가르키게 됨
    ThreadedPtr tmp;
    tmp = tree->left_child;
    while(!tmp->right_thread){
        tmp = tmp->right_child;
    }
    return tmp;
}
ThreadedPtr InorderSucc(ThreadedPtr tree){ //오른쪽 subtree의 가장 왼쪽 값이 가르키게 됨
    ThreadedPtr tmp;
    tmp = tree->right_child;
    while(!tmp->left_thread){
            tmp = tmp->left_child;
    }
    return tmp;
}
void ConnectThread(ThreadedPtr tree){ //스레드 연결시키기. 단 가장 왼쪽 값과 가장 오른쪽 스레드는 별도로 작업
        if(!tree->right_thread){
            InorderSucc(tree)->left_child = tree;
        }
        if(!tree->left_thread){
            InorderPred(tree)->right_child = tree;
        }
}
void activateCT(ThreadedPtr root, int root_idx, int idx){
    ThreadedPtr ptr = root; //root부터 시작해서 idx까지 따라가는 포인터
    //이진수를 담을 배열의 크기를 구하자.
    int tmp = idx;
    int count = 0; //배열의 크기
    while(1){
        tmp /= 2;
        count++;
        if(tmp == 0) break;
    }
    int arr[count]; //이진수를 담을 배열
    for(int i = 0; i < count; i++){ //배열에 이진수를 차례대로 넣음
        arr[count-1-i] = idx%2;
        idx/=2;
    }
    if(count != 1){
        for(int i = root_idx; i < count - 1; i++){ //idx 노드의 부모 노드를 ptr이 가르킴
            if(arr[i] == 1){ //1인 경우는 오른쪽
                ptr = ptr -> right_child;
            }else if(arr[i] == 0){ //0인 경우는 왼쪽
                ptr = ptr -> left_child;
            }
        }
    }//count가 1인경우는 ptr이 root를 가리킴
    if(arr[count-1] == 1){ //1일 경우 오른쪽에 대해서 ConnectThread 호출
        ConnectThread(ptr->right_child);
    }else if(arr[count-1] == 0){ //0일 경우 왼쪽에 대해서 ConnectThread 호출
        ConnectThread(ptr->left_child);
    }
}

void printInorder(ThreadedPtr root){
    ThreadedPtr tmp = InorderSucc(root);
    fprintf(fout, "inorder traversal :");
    while(1){
        if(tmp == root)break;
        if(tmp->right_thread){ //오른쪽 child가 없다면 thread를 통해 이동
            fprintf(fout, " %d", tmp->data);
            tmp = tmp->right_child;
        }else{ //오른쪽 child가 있다면 InorderSuccessor로 이동
            fprintf(fout, " %d", tmp->data);
            tmp = InorderSucc(tmp);
        }
    }
}
void DeleteTree(ThreadedPtr root){ //malloc 했던 메모리들 삭제
    ThreadedPtr tmp = InorderSucc(root);
    ThreadedPtr ptr = tmp;

    while(1){
        if(tmp == root){
            free(ptr);
            break;
        }
        if(tmp->right_thread){
            tmp = tmp->right_child;
            free(ptr);
            ptr = tmp;
        }else{
            tmp = InorderSucc(tmp);
            free(ptr);
            ptr = tmp;
        }
    }
}
