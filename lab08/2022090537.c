#include<stdio.h>
#include<stdlib.h>
#define INF 1e9


FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
int Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
int* GetElements(Heap* heap);
int IsFull(Heap *heap);
int IsEmpty(Heap *heap);
int Size(Heap *heap);

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;
	int ret;
	int* elements;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				ret = Insert(maxHeap, key);
				if(ret == 0) fprintf(fout, "insert %d\n", key);
				else if(ret == 1) fprintf(fout, "insert error : heap is full\n");
				else if(ret == 2) fprintf(fout, "insert error : %d is already in the heap\n", key);
				else {
					fprintf(fout, "error: unknown return type\n");
					return -1;
				} 

				break;
			case 'd':
				max_element = DeleteMax(maxHeap);
				if(max_element == 0){
					fprintf(fout, "delete error : heap is empty\n");

				}
				else if(max_element > 0){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				if(IsEmpty(maxHeap)){
					fprintf(fout, "print error : heap is empty\n");
				}else{
					elements = GetElements(maxHeap);
					int size = Size(maxHeap);
					for(int i = 0; elements[i] > -1 && i < size; i++){
						fprintf(fout, "%d ", elements[i]);
					}
					fprintf(fout, "\n");
					free(elements);
				}
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "finding error : %d is not in the heap\n", key);
				break;
		}
	}

	return 0;
}

/*
Create new heap with given heapSize
Element[0] should save INF
heapSize: positive integer
return:
	the pointer of new heap  
*/
Heap* CreateHeap(int heapSize){
	Heap* heap = (Heap*) malloc(sizeof(struct HeapStruct));
	heap -> Capacity = heapSize;
	heap -> Elements = (int*) malloc(sizeof(int) * (heapSize + 1));
	heap -> Size = 0;

	heap -> Elements[0] = INF;
	return heap;
}
/*
heap->Capacity를 heapSize로 초기화
heap->Capacity 만큼의 int형 메모리를 heap->Elements에 동적 할당
heap->Size를 0으로 초기화한 뒤 heap의 포인터를 반환
*/

 
/*
Insert the value into Heap
value: positive integer
return:
	0 , success
	1 , heap is full
	2 , duplicated
*/
int Insert(Heap *heap, int value){
	if(IsFull(heap)){
		return 1;
	}

	if(heap -> Size == 0){
		heap -> Elements[++heap->Size] = value;
		return 0;
	}
	if(Find(heap, value)){
		return 2; 
	}

	int i = ++heap->Size;

	while((i != 0) && heap->Elements[i / 2] < value){//percolating up
		heap->Elements[i] = heap->Elements[i/2];
		i /= 2;
	}
	
	heap -> Elements[i] = value;

	return 0;
}
/*
IsFull 함수를 통해 힙이 가득 차 있으면 어떠한 삽입 동작 없이 1을 반환하고 종료
heap->Size == 0일 경우(힙이 비었을 경우), heap->Elements[1]에 key를 삽입한 뒤 0을 반환하고 종료
Find 함수를 통해 value가 힙 안에 이미 존재할 경우, 어떠한 삽입 동작 없이 2를 반환하고 종료

삽입하고자 하는 key 값(value)이 부모노드의 key 값 보다 클 경우 부모 노드와 자식 노드의 key 값을 교환
*/

/*
Find the value in Heap
return:
	1, success
	0, fail  
*/


int Find(Heap* heap, int value){
	for(int i = 1; i <= heap->Size; i++){
		if(heap->Elements[i] == value){
		return 1;
		}
	}
	return 0;
}
/*
value가 힙 안에 존재하면 1을 반환. 존재하지 않으면 0을 반환
*/


/*
Delete the maximum value in Heap
return:
	Max value, success
	0, heap is empty  
*/
int DeleteMax(Heap* heap){
	if(IsEmpty(heap)){
		return 0;
	}

	int i, child;
	int max, last;

	max = heap -> Elements[1];
	last = heap -> Elements[heap->Size--]; // heap->Size는 하나 감소 - Max 값을 없애기 때문에 크기가 감소

	for(i = 1; i * 2 <= heap -> Size; i = child){// percolating down
		child = i * 2;
		if(child < heap->Size && heap -> Elements[child] < heap -> Elements[child + 1]){
			child++;
		} 
		if(last < heap -> Elements[child]){
			heap->Elements[i] = heap -> Elements[child];
		}
		else{
			break;
		}
	}

	heap -> Elements[i] = last;
	return max;
}
/*

*/

/*
Return an array of copied elements from the heap
The length of the returned array should be the same as the size of the heap
The order of the returned array should be the same as the order of heap->Elements  
If heap is empty, return NULL
***WARNING***
Do not just return heap->Elements
You must copy the elements to another allocated array
*/
int* GetElements(Heap* heap){
	int* arr_tmp = malloc(sizeof(int)*(heap->Size));
	int cnt = 0;

	for(int i = 1; i <= heap->Size; i++){
		arr_tmp[cnt++] = heap->Elements[i];
	}

	return arr_tmp;
}

/*
check heap is full
return:
	1, heap is full
	0, heap is not full
*/
int IsFull(Heap* heap){
	return heap -> Capacity <= heap -> Size;
}

/*
check heap is empty
return:
	1, heap is empty
	0, heap is not empty
*/
int IsEmpty(Heap *heap){
	if(heap->Size == 0){
		return 1;
	}
	return 0;

}

/*
get size of heap
return:
	size of heap
*/
int Size(Heap *heap){
	return heap->Size;
}