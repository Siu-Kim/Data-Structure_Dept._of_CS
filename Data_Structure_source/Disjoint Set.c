#include<stdio.h>
#include<stdlib.h>
#include<time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
    int size_maze;
    int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);
int check(DisjointSets *sets, int num);

int main(int argc, char* argv[]){
    srand((unsigned int)time(NULL));

    int num, i;
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);
 
    sets = (DisjointSets*)malloc(sizeof(DisjointSets));
    maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

    init(sets, maze_print, num);
    createMaze(sets, maze_print, num);
    printMaze(maze_print, num);

    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}

void init(DisjointSets *sets, DisjointSets *maze_print, int num){
    int i;

    sets->size_maze = num * num;
    sets->ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze + 1));
    for(i = 1; i <= sets->size_maze; i++) sets->ptr_arr[i] = i;

    maze_print->size_maze = num * num * 2;
    maze_print->ptr_arr = (int*)malloc(sizeof(int) * (maze_print->size_maze + 1));
    for(i = 1; i <= maze_print->size_maze; i++) maze_print->ptr_arr[i] = 1;

    maze_print->ptr_arr[maze_print->size_maze - 1] = 0;
}

void Union(DisjointSets *sets, int i, int j){
    int x = find(sets, i); // x는 i가 속하는 트리의 부모 인덱스
    int y = find(sets, j); // y는 j가 속하는 트리의 부모 인덱스

    if(sets->ptr_arr[x] < sets->ptr_arr[y]){
        sets->ptr_arr[y] = x;
    }else{
        sets -> ptr_arr[x] = y;
    }
}

int find(DisjointSets *sets, int i){
    int x = sets -> ptr_arr[i];
    while(x != i){
        return find(sets, x);
    }
    return x; //root 값을 반환
}

void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num){
    while(check(sets, num) == 0){
        int randomplace = rand() % (num * num) + 1; //방의 위치를 1~num*num 사이의 수로 random하게 받음
        int randomwall = rand() % 2; //0이 아래 1이 오른쪽
        if(randomplace % num == 0 && randomwall == 1){ //맨 오른쪽에 위치한 maze면서 오른쪽 벽일 경우
            continue;
        }
        if(randomplace > num*num-num && randomwall == 0){ //맨 아래쪽에 위치한 maze면서 아래쪽 벽일경우
            continue;
        }
        if(randomwall == 0){ //위의 두 상황이 아닐 때 아래벽일 경우
            if(find(sets, randomplace) != find(sets, randomplace + num)){
                Union(sets, randomplace, randomplace + num);
                maze_print->ptr_arr[randomplace*2] = 0;
            }
        }else{ //위의 두 상황이 아닐 때 오른쪽 벽일 경우
            if(find(sets, randomplace) != find(sets, randomplace + 1)){
                Union(sets, randomplace, randomplace + 1);
                maze_print->ptr_arr[randomplace*2-1] = 0;
            }
        }
    }
}

void printMaze(DisjointSets *sets, int num){
    for(int i = 0; i < num; i++){ //맨 윗줄 출력
        fprintf(fout, "+---");
    }
    fprintf(fout, "+\n");
    int p = 1;

    for(int i = 1; i <= num * 2; i++){ //세로 벽을 포함하는 줄과 가로벽을 포함하는 줄을 분리해서 각각 만듦
        if(i % 2 == 1){ //세로 벽을 포함하는 줄
            if(i == 1){
                fprintf(fout, " "); //출발하는 지점은 뚫려있음
            }else{
                fprintf(fout, "|"); //i가 1이 아님, 즉 출발하는 지점을 제외한 나머지 지점들의 시작점
            }
            for(int j = p; j < p+num; j++){
                if(sets->ptr_arr[2*(j)-1] == 1){ //세로벽 만들기
                    fprintf(fout, "   |");
                }else{
                    fprintf(fout, "    ");
                }
            }
            fprintf(fout, "\n");
        }else{ //가로벽을 포함하는 줄
            if(i == num * 2){ //맨 아랫줄일 경우
                fprintf(fout, "+");
                for(int k = 0; k < num; k++){
                    fprintf(fout, "---+");
                }
            }else{
                fprintf(fout, "+");
                for(int j = p; j < p+num; j++){
                    if(sets->ptr_arr[2*j] == 1){//가로벽 만들기
                        fprintf(fout, "---+");
                    }else{
                        fprintf(fout, "   +");
                    }
                }
                fprintf(fout, "\n");
            }
            p += num;
        }
    }
}


void freeMaze(DisjointSets *sets, DisjointSets *maze_print){ //malloc한 것들 다 free하기
    free(sets->ptr_arr);
    free(maze_print->ptr_arr);
    free(sets);
    free(maze_print);
}

int check(DisjointSets *sets, int num){
    int checking = find(sets, 1);
    for(int i = 1; i <= num * num; i++){
        if(find(sets, i) != checking){
            return 0; // 같은 트리에 속하지 않는 값이 하나라도 존재한다면 0을 반환
        }
    }
    return 1; //모두 같은 트리에 속하면 1을 반환
}
