#include <stdio.h>
#include <stdlib.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
    int Capacity; //힙의 최대 용량
    int Size; //현재 엘리먼트 개수
    int *Elements; //배열(인덱스는 1부터 유효)
}Heap;

Heap* CreateHeap(int heapSize);
void Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);
int IsFull(Heap *heap);
void DeleteHeap(Heap *heap);

int main(int argc, char* argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char cv;
    Heap* maxHeap;
    int heapSize, key, max_element;

    while(EOF != fscanf(fin, "%c", &cv)){
        switch(cv){
            case 'n': //힙생성
                fscanf(fin, "%d", &heapSize);
                maxHeap = CreateHeap(heapSize);
                break;
            case 'i': //key값 추가
                fscanf(fin, "%d", &key);
                Insert(maxHeap, key);
                break;
            case 'd': //max_element 삭제
                max_element = DeleteMax(maxHeap); //힙이 비어있다면 -INF반환
                if(max_element != -INF){
                    fprintf(fout, "max element : %d deleted\n", max_element);
                }
                break;
            case 'p': //힙에 들어있는 값들 print
                PrintHeap(maxHeap);
                break;
            case 'f': //key 값이 힙에 존재하는지 확인
                fscanf(fin, "%d", &key);
                if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
                else fprintf(fout, "finding error : %d is not in the heap\n", key);
                break;
        }
    }
    DeleteHeap(maxHeap); //힙을 메모리에서 free
    return 0;
}

Heap* CreateHeap(int heapSize){
    Heap* heap = malloc(sizeof(struct HeapStruct));
    heap->Capacity = heapSize; //힙의 최대 용량
    heap->Size = 0; //힙의 현재 엘리먼트 개수
    heap->Elements = malloc((heapSize+1)*sizeof(int)); //인덱스 0은 비워둘 것이기 때문에 heapSize + 1 만큼으로 크기 설정
    heap->Elements[0] = INF; //insert 과정에서 0번째 인덱스와 비교할 때 무조건 0번째 인덱스가 더 커야하므로 INF로 설정
                            // 이 과정을 진행 안하면 포인터 변수의 주소가 할당되며 그 주소와 1번째 인덱스를 비교하게 됨
    return heap;
}
void Insert(Heap *heap, int value){
    int i;
    if(IsFull(heap)){ //힙이 Full인 경우 insert 에러
        fprintf(fout, "insert error : heap is full\n");
    }else {
        if(Find(heap, value)){ //이미 값이 힙에 존재하는 경우 insert 에러
            fprintf(fout, "insert error : %d is already in the heap\n", value);
        }else{ //insert
            for(i = ++heap->Size; heap->Elements[i/2] < value; i/=2){ //percolating up
                heap->Elements[i] = heap->Elements[i/2];
            }
            heap->Elements[i] = value;
            fprintf(fout, "insert %d\n", value);
        }
    }
}
int Find(Heap *heap, int value){ //힙에 value가 존재하는지 확인
    for(int i = 1; i <= heap->Size; i++){
        if(heap->Elements[i] == value) return 1;
    }
    return 0;
}
int DeleteMax(Heap* heap){
    if(heap->Size == 0){ //힙이 비어있는 경우 삭제할 엘리먼트가 없음
        fprintf(fout, "delete error : heap is empty\n");
        return -INF;
    }else{ //힙이 비어있지 않은 경우
        int i, child;
        int max_element, last_element;

        max_element = heap->Elements[1];
        last_element = heap->Elements[heap->Size--]; //힙 사이즈는 하나 줄어듦(Max값을 없애기 때문)
        for(i = 1; i*2 <= heap->Size; i=child){ //percolating down
            child = i*2;
            if(child < heap->Size && heap->Elements[child] < heap->Elements[child+1]){
                child++; //더 큰 값의 child를 결정함
            }
            if(heap->Elements[child] > last_element){ //child와 last_element중 child가 더 큰 경우
                heap->Elements[i] = heap->Elements[child];
            }else{
                break;
            }
        }
        heap->Elements[i] = last_element; //last_element를 넣어주고 종료
        return max_element;
    }
}
void PrintHeap(Heap* heap){
    if(heap->Size == 0){ //힙이 비어있는 경우 print error
        fprintf(fout, "print error : heap is empty\n");
    }else{
        for(int i = 1; i <= heap->Size; i++){
            fprintf(fout, "%d ", heap->Elements[i]); //level order traversal로 출력(인덱스 순서대로)
        }
        fprintf(fout, "\n"); //print를 다 한 후 개행
    }
}
int IsFull(Heap *heap){ //힙이 꽉찼는지 확인
    return heap->Capacity <= heap->Size;
}
void DeleteHeap(Heap *heap){ //힙을 메모리에서 free
    free(heap->Elements);
    free(heap);
}
