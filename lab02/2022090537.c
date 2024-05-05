#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node{
	ElementType element;
	Position next;
};

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
Position FindPrevious(ElementType X, List L);
Position Find(ElementType X, List L);
void Delete(ElementType X, List L);
void DeleteList(List L);
int* GetElements(List L);
int ListSize=0;

int main(int argc, char **argv){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");
	char x;
	int* elements;

	Position header=NULL, tmp=NULL;
	header = MakeEmpty(header); //key = -1인 header node 생성
	while(fscanf(fin,"%c",&x)!=EOF){
		if(x=='i'){
			int a,b;	fscanf(fin,"%d%d",&a,&b);
			tmp = Find(a, header);
			if(tmp!=NULL){
				fprintf(fout, "insertion(%d) failed : the key already exists in the list\n", a);
				continue;
			}
			tmp = Find(b, header);
			if(tmp==NULL){
				fprintf(fout, "insertion(%d) failed : can not find the location to be inserted\n", a);
				continue;
			}
			Insert(a, header, tmp);
		}
		else if(x=='d'){
			int a;	fscanf(fin,"%d",&a);
			tmp = Find(a, header);
			if(tmp==NULL){
				fprintf(fout, "deletion(%d) failed : element %d is not in the list\n", a, a);
				continue;
			}
			Delete(a, header);
		}
		else if(x=='f'){
			int a;	fscanf(fin,"%d",&a);
			tmp = FindPrevious(a, header);
			if(IsLast(tmp, header)) fprintf(fout, "finding(%d) failed : element %d is not in the list\n",a,a);
			else{
				if(tmp->element>0) fprintf(fout, "key of the previous node of %d is %d\n", a, tmp->element);
				else fprintf(fout, "key of the previous node of %d is head\n",a);
			}
		}
		else if(x=='p') {
			if(IsEmpty(header))
				fprintf(fout, "empty list!\n");
			else
			{
				elements = GetElements(header);
				for(int i = 0; i < ListSize; i++){
						fprintf(fout, "key:%d ", elements[i]);
					}
					fprintf(fout, "\n");
				free(elements);
			}
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);

	return 0;
}

List MakeEmpty(List L){
	L = (List) malloc(sizeof(struct Node)); //struct Node type
	L -> element = -1; //header의 key는 -1
	L -> next = NULL;
	return L; //header의 구조체 포인터 반환 
/*
Make new header
element should be -1
return:
	the pointer of new list
*/
}

int IsEmpty(List L){
	return L->next == NULL;
	/* L(header)의 next node가 존재하지 않으면 1,
	next node가 존재하면 0 출력 */
/*
Check if list L is empty or not
return:
	1, list is full
	0, list is not full
*/
}

int IsLast(Position P, List L){
	return P->next == NULL;
	/* P의 next node가 존재하지 않으면 1,
	next node가 존재하면 0 출력 */
/*
Check if Position P is last or not
return:
	1, list is full
	0, list is not full
*/
}

void Insert(ElementType X, List L, Position P){
	Position tmp;
	tmp = (Position) malloc(sizeof(struct Node)); //struct Node만큼의 메모리 할당
	tmp -> element = X; //삽입할 새 node -> element에 X 대입
	tmp -> next = P -> next; //새 node -> next에 삽입할 위치의 node -> next 대입 (삽입하는 위치가 list의 가장 뒤쪽일 경우, next = NULL)
	P -> next = tmp; //삽입할 위치의 node -> next에 새 node의 주소 대입
	ListSize ++; //노드 개수(ListSize)를 하나씩 증가
/*
Insert X in position P of list L
*/
}

int* GetElements(List L){
	int* key_array = (int*)malloc(sizeof(int)*ListSize); //int(4byte) * ListSize 만큼의 메모리 할당
	Position tmp;
	int i = 0;
	tmp = L->next; // tmp = header -> next로 설정. header node 제외 가장 앞에 있는 node부터 탐색

	while(tmp != NULL){ //탐색 중인 linked list의 node를 벗어나기 전까지 반복
		key_array[i] = tmp->element;
		tmp = tmp -> next;
		i++;
	}

	return key_array;
/*
Return an array of copied elements from the List L
The length of the returned array should be the same as the size of the ListSize
The order of the returned array should be the same as the order of L->element  
If List is empty, return NULL
***WARNING***
You must copy the elements to another allocated array
*/
}

Position FindPrevious(ElementType X, List L){
	Position p;
	p = L; //p = header node header부터 탐색
	while(p -> next != NULL && p -> next ->element != X){ //마지막 node이거나 찾고자 하는 key를 가진 node의 전 node를 탐색 중이면 반복문 탈출
		p = p->next;
	}
	return p;
/*
Find the previous node of the node with the given element X in List L
return:
	the pointer of previous node of node that has element X  
*/
}

Position Find(ElementType X, List L){
	Position p;

	p = L; //p = header node
	while (p != NULL && p -> element != X){ //list 내에 element로 X를 가지는 node가 존재하지 않으면 list의 끝까지 탐색 후 NULL 반환
		p = p-> next;
	}
	return p;
/*
Find the node with the given element X in List L
return:
	the pointer of node has element X
*/
}

void Delete(ElementType X, List L){
	Position p, tmp;
	p = FindPrevious(X, L); //element로 X를 가진 node의 이전 node를 반환
	if (!IsLast(p, L)) { 
		tmp = p -> next; //tmp에 delete하고자 하는 node의 대입
		p -> next = tmp -> next; //이전 node -> next에 next node의 주소를 대입. tmp의 메모리 주소 소실
		free(tmp); //할당된 메모리 해제
		ListSize --; //list 내의 node의 개수(ListSize)를 1만큼 감소
		}
/*
Delete the node that has element X in List L
*/
}

void DeleteList(List L){
	Position p, tmp;

	p = L->next; //header 다음 노드 대입
	L->next = NULL; //header의 next = NULL
	while(p != NULL){ // header 다음 node부터 list 범위를 벗어날 때까지 탐색
		tmp = p->next; //다음 node의 주소를 tmp에 저장 후 현재 node에 할당된 메모리 해제
		free(p);
		p = tmp;
	}
/*
Delete the List
*/
}