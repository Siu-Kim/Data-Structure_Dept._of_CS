#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl* HashTable;
typedef struct HashTbl{
	int TableSize;
	List *TheLists;
}HashTbl;


HashTable createTable(int TableSize);
void Insert(HashTable H, ElementType Key, int solution);
void Delete(HashTable H, ElementType Key, int solution);
int Find(HashTable H, ElementType Key, int solution);
void printTable(HashTable H);
void deleteTable(HashTable H);
int IsFull(HashTable H);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");


	char solution_str[20];
	int solution, TableSize;

	fscanf(fin, "%s", solution_str);
	if(!strcmp(solution_str, "linear"))
		solution = 1;
	else if(!strcmp(solution_str, "quadratic"))
		solution = 2;
	else{
		fprintf(fout, "Error: Invalid hashing solution!");
		return 0;
	}
	
	fscanf(fin, "%d", &TableSize);

	HashTable H = createTable(TableSize);

	char cv;
	int key;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				Insert(H, key, solution);
				break;

			case 'f':
				fscanf(fin, "%d", &key);
				int result = Find(H, key, solution);
				if(result)
					fprintf(fout, "%d is in the table\n", key);
				else
					fprintf(fout, "%d is not in the table\n", key);
				break;

			case 'd':
				fscanf(fin, "%d", &key);
				Delete(H, key, solution);
				break;

			case 'p':
				printTable(H);
				break;
		}
	}

	deleteTable(H);

	return 0;
}

/*
Create new Hash Table with given TableSize
reture:
	the pointer of new hash table
*/
HashTable createTable(int TableSize){
	HashTable table = malloc(sizeof(HashTable));

	table->TableSize = TableSize;
	table->TheLists = malloc(sizeof(int) * table->TableSize);

	for(int i = 0; i < table -> TableSize; i++){
		table -> TheLists[i] = 0; //비어있는 bucket은 전부 0으로 초기화(DEL 동작이 수행된 경우 -1)
	}
	
	return table;
}

/*
Insert the key in hash table with given solution (linear or quadratic).
print out : 
	Successful insertion : "insert %d into address %d\n"
	When inserting duplicate key value : "insertion error : %d already exists at address %d\n"
	When HashTable is full : "insertion error : table is full\n"
*/
void Insert(HashTable H, ElementType Key, int solution){
	int i = 0;
	printf("debug\n\n");

	if(IsFull(H)){ // Hash table이 가득 차 있을 경우
		fprintf(fout, "insertion error: table is full\n");
		
		return;
	}

	if(Find(H, Key, solution)){ // insert 하는 value가 이미 hash 안에 존재할 경우
		int idx;
		
		for(int i = 0; i < H->TableSize; i++){
			if(H->TheLists[i] == Key) idx = i; break;
			printf("debug 01 ");
		}
		printf("debug 02\n\n");
		fprintf(fout, "insertion error: %d already exists at address %d\n", Key, idx);
		return;
	}

	switch (solution) // 빈 bucket은 0으로 초기화, DEL slot은 -1로 초기화
	{
		case 1: // Linear proving
			while(1){
				if(H -> TheLists[(i + Key) % (H -> TableSize)] == 0 || H -> TheLists[(i + Key) % (H -> TableSize)] == -1){
					H -> TheLists[(i + Key) % (H -> TableSize)] = Key;

					fprintf(fout, "insert %d into address %d\n", Key, (i + Key ) % (H -> TableSize));
					return;
				}
				i++;
				printf("loop %d", i);

				if(i == H -> TableSize) break;
			}

			return;
	
		case 2: // Quadratic proving
			while(1){
				if(H -> TheLists[(i*i + Key) % (H -> TableSize)] == 0 || H -> TheLists[(i*i + Key) % (H -> TableSize)] == -1){
					H -> TheLists[(i*i + Key) % (H -> TableSize)] = Key;

					fprintf(fout, "insert %d into address %d\n", Key, (i * i + Key) % (H -> TableSize));
					return;
				}
				i++;
				printf("loop %d", i);

				if(i == H -> TableSize) break;
			}

			return;
	}
}


/*
Delete the key in hash table with given solution (linear or quadratic).
print out : 	
	when key value is found :  "delete %d\n"
	when key value does not exist : %d is not in the table\n"
*/
void Delete(HashTable H, ElementType Key, int solution){
	if(!Find(H, Key, solution)){
		fprintf(fout, "deletion error: %d is not in the table\n", Key);
		return;
	}

	int i = 0;

	switch(solution)
	{
	case 1:
		while(1){
			if(H -> TheLists[(i + Key) % (H -> TableSize)] == Key){
				H -> TheLists[(i + Key) % (H -> TableSize)]	= -1;

				fprintf(fout, "delete %d\n", Key);
				return;
			}
			i++;
		}
	
	case 2:
		while(1){
			if(H -> TheLists[(i * i + Key) % (H -> TableSize)] == Key){
				H -> TheLists[(i * i + Key) % (H -> TableSize)] = -1;

				fprintf(fout, "delete %d\n", Key);
				return;
			}
			i++;
		}	
	}
}


/*
Find the Key from hash table with given solution (linear or quadratic).
return:
	0 : when the key is not found
	1 : when the key is found
*/
int Find(HashTable H, ElementType Key, int solution){
	int i = 0;
	int start_point = Key % H -> TableSize;

	switch (solution)
	{
	case 1:
		while(1){
			if(i == H -> TableSize) return 0; // hash table을 모두 탐색 -> table 안에 존재하지 않음

			else if(H -> TheLists[(i + Key) % (H -> TableSize)] == 0) return 0; //해당 bucket이 비어있음(Key로 index하기 때문에 bucket이 비어있다면 hash table 안에 존재하지 않음)
			else if(H -> TheLists[(i + Key) % (H -> TableSize)] == Key) return 1; //table안에 Key가 존재함
		}
		i++;
	
	case 2: // case 1과 동일(linear proving과 quadratic proving의 차이만 존재)
		while(1){
			if(i == H -> TableSize) return 0; // table 모두 탐색

			else if(H -> TheLists[(i * i + Key) % (H -> TableSize)] == 0) return 0; // table 내에 존재하지 않음
			else if(H -> TheLists[(i * i + Key) % (H -> TableSize)] == Key) return 1; // Key가 table 내에 존재
		
		}
		i++;
	}
}


/*
Print all values of Hash Table
print out the key values ​​in the Hash Table in the order of index in the Hash Table.
	empty Hash Table print : "0 "
	Non empty Hash Table print : "%d "
*/
void printTable(HashTable H){
	for(int i = 0; i < H -> TableSize; i++){
		if(H -> TheLists[i] == -1){ // DEL 값이 저장된 bucket은 0으로 값을 바꿔 출력(실제 -1을 hash에 저장하는 것이 아니라 Delete 동작 이후, 비어있는 bucket을 구분하기 위한 것이기 때문)
			fprintf(fout, "0 ");
		}
		else{
			fprintf(fout, "%d ", H -> TheLists[i]);
		}
	}
	fprintf(fout, "\n");
}


/*
delete Table 
*/
void deleteTable(HashTable H){ // HashTable H -> TheLists에 할당된 메모리를 free => 구조체 H를 free
	free(H -> TheLists);
	free(H);
}

/*
hash table이 비어있는지 확인
table이 가득 찬 경우, 1 반환
table이 가득 차지 않은 경우, 0 반환
*/
int IsFull(HashTable H){
	for(int i = 0; i < H -> TableSize; i++){
		if(H -> TheLists[i] == 0 || H -> TheLists[i] == -1) return 0;
	}

	return 1;
}