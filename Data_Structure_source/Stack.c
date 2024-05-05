#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack{
    int *key;
    int top;
    int max_stack_size;
}Stack;


Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

void main(int argc, char *argv[]){ //int return 값을 void로 고칠 것
    fin=fopen(argv[1], "r");
    fout=fopen(argv[2], "w"); 
    Stack* stack;
    char input_str[101];
    int max=20, i=0, a, b, result, error_flag=0;

    fgets(input_str, 101, fin); //최대 100개의 characters를 읽고 input_str에 넣음
    stack = CreateStack(max); //길이가 20인 스택을 생성

    fprintf(fout, "top numbers : ");
    while(input_str[i]!='#'){
        if(0 < input_str[i] - '0' && input_str[i] - '0' < 10){
             if(IsFull(stack)){ //Push를 실행하기 이전에 스택이 꽉찼는지 확인
                error_flag = 1;
                break;
            }
            Push(stack, input_str[i] - '0'); //다음 입력값이 숫자일 경우
        }else if(input_str[i] == '+'){ // 다음 입력값이 연산자일 경우
            if(IsEmpty(stack)){ //Pop을 실행하기 이전에 스택이 비었는지 확인
                error_flag = 2;
                break;
            }
            b = Pop(stack);
            if(IsEmpty(stack)){ //Pop을 실행하기 이전에 스택이 비었는지 확인
                error_flag = 2;
                break;
            }
            a = Pop(stack);
            Push(stack, a+b);
        }else if(input_str[i] == '-'){
            if(IsEmpty(stack)){ //Pop을 실행하기 이전에 스택이 비었는지 확인
                error_flag = 2;
                break;
            }
            b = Pop(stack);
            if(IsEmpty(stack)){ //Pop을 실행하기 이전에 스택이 비었는지 확인
                error_flag = 2;
                break;
            }
            a = Pop(stack);
            Push(stack, a-b);
        }else if(input_str[i] == '*'){
            if(IsEmpty(stack)){ //Pop을 실행하기 이전에 스택이 비었는지 확인
                error_flag = 2;
                break;
            }
            b = Pop(stack);
            if(IsEmpty(stack)){ //Pop을 실행하기 이전에 스택이 비었는지 확인
                error_flag = 2;
                break;
            }
            a = Pop(stack);
            Push(stack, a*b);
        }else if(input_str[i] == '/'){
            if(IsEmpty(stack)){ //Pop을 실행하기 이전에 스택이 비었는지 확인
                error_flag = 2;
                break;
            }
            b = Pop(stack);
            if(IsEmpty(stack)){ //Pop을 실행하기 이전에 스택이 비었는지 확인
                error_flag = 2;
                break;
            }
            a = Pop(stack);
            if(b == 0){ //나누기 연산을 진행하기 전에 b가 0인지 여부 확인
                error_flag = 3;
                break;
            }
            Push(stack, a/b);
        }else if(input_str[i] == '%'){
            if(IsEmpty(stack)){ //Pop을 실행하기 이전에 스택이 비었는지 확인
                error_flag = 2;
                break;
            }
            b = Pop(stack);
            if(IsEmpty(stack)){ //Pop을 실행하기 이전에 스택이 비었는지 확인
                error_flag = 2;
                break;
            }
            a = Pop(stack);
            if(b == 0){ //나누기 연산을 진행하기 전에 b가 0인지 여부 확인
                error_flag = 3;
                break;
            }
            Push(stack, a%b);
        }

        int t = Top(stack); //연산을 진행한 후의 값
        fprintf(fout, "%d ", t);
        result = t;

        i++;
    }
    if(error_flag == ISFULL){
        fprintf(fout, "\nerror : invalid postfix expression, stack is full\n");
    }
    else if(error_flag == ISEMPTY){
        fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
    }
    else if(error_flag == DIVIDEZERO){
        fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
    }
    else{
        if(stack->top+1 > 1){
            fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top+1);
        }
        else{
            fprintf(fout, "\nevaluation result : %d\n", result);
        }
    }

    fclose(fin);
    fclose(fout);
    DeleteStack(stack); 
}

Stack* CreateStack(int max){
    Stack* s;
    s = (Stack*)malloc(sizeof(struct Stack));
    s->key = (int*)malloc(sizeof(int) * max);
    s->max_stack_size = max;
    s->top = -1; //헤더의 top은 -1 지정
    return s;
}
void Push(Stack* S, int X){
    S->key[++(S->top)] = X;
}
int Pop(Stack* S){
    int tmp = S->top;
    S->top--;
    return S->key[tmp];
}
int Top(Stack* S){
    return S->key[S->top];
}

void DeleteStack(Stack* S){
    free(S->key);
    free(S);
}
int IsEmpty(Stack* S){
    return S->top == -1;
}
int IsFull(Stack* S){
    return S->top == 19;
}

