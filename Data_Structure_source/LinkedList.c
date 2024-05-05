#include<stdlib.h>
#include<stdio.h>
#include<string.h>

FILE* input;
FILE* output;

typedef struct Node* PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
void PrintList(List L);
void Delete(ElementType X, List L);
Position Find(ElementType X, List);
Position Find(ElementType X, List);
Position FindPrevious(ElementType X, List L);
void DeleteList(List L);

struct Node {
    ElementType element;
    Position next;
};
int main(int argc, char* argv[]) {
    char command;
    int key1, key2;
    FILE* input;
    Position header = NULL;
    Position tmp = NULL;
    if (argc == 1) {
        input = fopen("input.txt", "r");
        output = fopen("output.txt", "w");
    }
    else {
        input = fopen(argv[1], "r");
        output = fopen(argv[2], "w");
    }
    header = MakeEmpty(header);
    while (1) {
        command = fgetc(input);
        if (feof(input)) break;
        switch (command) {
        case 'i':
            fscanf(input, "%d %d", &key1, &key2);
            if (key2 == -1) { //헤더 뒤에 값을 추가
                Insert(key1, header, header);
                break;
            }
            tmp = Find(key1, header); //key1 과 일치하는 값이 있는지 조사.
            if (tmp != NULL) {
                fprintf(output, "insertion(%d) failed : the key already exists in the list\n", key1);
                break;
            }
            tmp = Find(key2, header); //key2 값을 가르키는 포인터를 반환
            if (tmp == NULL) {
                fprintf(output, "insertion(%d) failed : can not find the location to be inserted\n", key1);
                break;
            }
            tmp = Find(key2, header);
            Insert(key1, header, tmp); //key1의 값을 key2(tmp)의 값의 뒤에 추가
            break;
        case 'd':
            fscanf(input, "%d", &key1);
            Delete(key1, header); //key1 노드를 삭제
            break;
        case 'f':
            fscanf(input, "%d", &key1);
            tmp = FindPrevious(key1, header);
            if (IsLast(tmp, header)) {
                fprintf(output, "finding(%d) failed: element %d in not in the list\n", key1, key1);
                break;
            }
            else {
                if (tmp->element > 0) {
                    fprintf(output, "key of the previous node of %d is %d\n", key1, tmp->element);
                }
                else {
                    fprintf(output, "key of the previous node of %d is head\n", key1);
                }
                break;
            }
        case 'p':
            PrintList(header);
            break;
        default:
            break;
        }
    }
    DeleteList(header);
    fclose(input);
    fclose(output);
    return 0;
}

List MakeEmpty(List L) { //헤더의 값을 -1로 지정하고 다음 노드를 NULL로 지정
    L = (List)malloc(sizeof(struct Node));
    L->element = -1;
    L->next = NULL;
    return L;
}
int IsEmpty(List L) {
    return L->next == NULL;
}
int IsLast(Position P, List L) {
    return P->next == NULL;
}
void Insert(ElementType X, List L, Position P) { //tmp노드는 insert하고자 하는 노드, tmp가 가르키는 노드를 p가 가르키는 노드로 만들고 p는 tmp를 가르키도록 함
    Position tmp;
    tmp = (Position)malloc(sizeof(struct Node));

    tmp->element = X;
    tmp->next = P->next;
    P->next = tmp;
}
void PrintList(List L) {
    while (L->next != NULL) {
        L = L->next;
        fprintf(output, "key:%d ", L->element);
    }
    fprintf(output, "\n");
}
void Delete(ElementType X, List L) {
    Position p, tmp;
    p = FindPrevious(X, L); //x의 이전노드
    if (!IsLast(p, L)) { //마지막이 아닐 경우
        tmp = p->next;
        p->next = tmp->next;
    }
    else //FindPrevious는 마지막까지 간 후 최종값을 반환하여 else문 작동
        fprintf(output, "deletion(%d) failed : element %d is not in the list\n", X, X);
}
Position Find(ElementType X, List L) {
    Position p;
    p = L;
    while (p->next != NULL && p->next->element != X) { //p 다음 노드가 존재하고 다음노드의 값이 X가 아닐 때 p를 다음 노드로 지정
        p = p->next;
    }
    p = p->next;//반복문 탈출 후 p를 다음 노드로 지정해야 p가 x에 해당하는 노드를 가르킴
    return p;
}
Position FindPrevious(ElementType X, List L) {
    Position p;
    p = L;
    while (p->next != NULL && p->next->element != X) { //p 다음 노드가 존재하고 그 값이 X가 아닐 때 반복
        p = p->next;
    }
    return p;//만약 최종까지 도달했을 때(다음 노드가 NULL)이면 가장 마지막 노드의 값을 반환
}
void DeleteList(List L) {
    Position p;
    p = L->next;
    L->next = NULL;
    while (p != NULL) {
        p = p->next;
    }
}
