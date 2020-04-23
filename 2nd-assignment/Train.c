#define MAX_STACK_SIZE 50
#define True 1
#define False 0
#define Error -1

#include <stdio.h>

typedef int Bool;
typedef int Element;
typedef struct {
    Element stackArr[MAX_STACK_SIZE];
    int top;
} Stack;

//스택 생성
Stack *Create(void) {
    Stack *tempstack; //구조체의 포인터
    tempstack = malloc(sizeof(Stack)); //스택을 동적할당
    tempstack->top = -1; //스택 초기화(top = -1)
    return tempstack;
}

//스택이 비어있는지 검사
Bool isEmpty(Stack *pstack) {
    if (pstack->top == -1)
        return True;
    else
        return False;
}

//스택이 다 찼는지 검사
Bool isFull(Stack *pstack) {
    if (pstack->top == MAX_STACK_SIZE - 1)
        return True;
    else
        return False;
}

//스택에 집어넣기
void Push(Stack *pstack, Element Data) {
    if (!isFull(pstack))
        pstack->stackArr[++pstack->top] = Data;
    else {
        printf("스택이 다 찼습니다.\n");
        exit(-1); //프로그램 종료
    }
}

//스택에서 끄집어내고 없애기
Element Pop(Stack *pstack) {
    if (!isEmpty(pstack))
        return pstack->stackArr[pstack->top--];
    else {
        printf("스택이 비어있습니다.\n");
        return Error;  //main함수에 오류 알린다.
    }
}

//스택에서 없애지 않고 끄집어내기
Element Peek(Stack *pstack) {
    if (!isEmpty(pstack))
        return pstack->stackArr[pstack->top];
    else {
        printf("스택이 비어있습니다!\n");
        return Error;  //main함수에 오류 알린다.
    }
}

int main() {
    char user[10]; //사용자가 입력하는 랜덤한 열차 순서
    int start[9];  //처음 랜덤한 열차 순서
    Stack *s1, *s2;   //스택1, 스택2
    int least1, least2; //스택1,2에서 최솟값
    int index1, index2;   //스택1,2에서 최솟값의 위치
    int count = 1, result = 1; //실행 횟수, 해당 결과 순서
    int i, j;
    int stackMax1 =0, stackMax2 =0;

    printf("열차 번호 입력 : ");
    gets(user);
    for (i = 0; i < 9; i++)
        start[i] = user[i] - '0'; //입력한 열차 순서를 숫자형태로 변형

    s1 = Create();
    s2 = Create(); //스택 생성

    i = 0;
    while (result < 10) { //마지막 순서 9가 결과로 올 때까지
        //least1, least2 초기화
        if (isEmpty(s1))  //스택이 비어있다면
            least1 = 10; //처음으로 초기화(스택이 비어있다는 뜻)
        else {  //스택에 원소가 있다면
            if (s1->top == 0) {  //스택에 1개의 원소만 있을 때(비교 필요x)
                least1 = s1->stackArr[s1->top];
                index1 = s1->top;
            }
            for (j = s1->top; j > 0 && (j - 1) >= 0; j--)  //스택 원소가 2개 이상일 때, 1번지와 0번지를 비교가 마지막 비교
                if (s1->stackArr[j] <= s1->stackArr[j - 1]) {
                    least1 = s1->stackArr[j];
                    index1 = j;
                } else {
                    least1 = s1->stackArr[j - 1];
                    index1 = j - 1;
                }
        }
        if (isEmpty(s2))
            least2 = 10;
        else {
            if (s2->top == 0) {
                least2 = s2->stackArr[s2->top];
                index2 = s2->top;
            }

            for (j = s2->top; j > 0 && (j - 1) >= 0; j--)
                if (s2->stackArr[j] <= s2->stackArr[j - 1]) {
                    least2 = s2->stackArr[j];
                    index2 = j;
                } else {
                    least2 = s2->stackArr[j - 1];
                    index2 = j - 1;
                }
        }

        //1. IN한 숫자가 바로 해당 숫자일 때
        if (i <= 8 && start[i] == result) {
            printf("%2d : IN(%d)\n", count++, start[i]);
            printf("%2d : OUT(%d)\n", count++, start[i]);

            result++; //바로 OUT
            i++; //입력한 다음 숫자 보기(IN)

        }
            //2. 스택1에 해당 숫자가 있을 때
        else if (least1 == result) {

            if (s1->stackArr[s1->top] == least1) { //만약 스택1의 top에 있었으면
                printf("%2d : POP(1)\n", count++);
                printf("%2d : OUT(%d)\n", count++, Pop(s1));
                result++; //POP 바로 OUT!

            } else {  //그렇지않고 스택1 중간에 있으면
                for (j = s1->top; j > index1; j--) {  //스택1의 해당 숫자 위에 있는 나머지들을 POP -> PUSH(스택2로)
                    printf("%2d : POP(1)\n", count++);
                    printf("%2d : PUSH(2,%d)\n", count++, s1->stackArr[j]);
                    Push(s2, Pop(s1));
                    if(s2->top+1 > stackMax2){
                        stackMax2 = s2->top+1;
                    }
                }
                printf("%2d : POP(1)\n", count++);
                printf("%2d : OUT(%d)\n", count++, Pop(s1));
                result++; //스택1의 top에 있는 숫자를 POP -> OUT
            }
        }
            //3. 스택2에 해당 숫자가 있을 때
        else if (least2 == result) {
            if (s2->stackArr[s2->top] == least2) { //만약 스택2의 top에 있었으면
                printf("%2d : POP(2)\n", count++);
                printf("%2d : OUT(%d)\n", count++, Pop(s2));
                result++; //POP 바로 OUT!
            } else {  //그렇지않고 스택2 중간에 있으면
                for (j = s2->top; j > index2; j--) { //스택2에서 해당 숫자 위에 있는 숫자들은 스택1로 POP -> PUSH
                    printf("%2d : POP(2)\n", count++);
                    printf("%2d : PUSH(1,%d)\n", count++, s2->stackArr[j]);
                    Push(s1, Pop(s2));
                    if(s1->top+1 > stackMax1){
                        stackMax1 = s1->top+1;
                    }
                }
                printf("%2d : POP(2)\n", count++);
                printf("%2d : OUT(%d)\n", count++, Pop(s2));
                result++; //스택2의 top에 있는 숫자를 POP -> OUT
            }
        }
            //4. 아직 해당 숫자가 안 나왔을 때 PUSH!(OUT없음)
        else if (i <= 8) {
            printf("%2d : IN(%d)\n", count++, start[i]); //IN()함수

            if (isEmpty(s1)) { //스택1이 비어있을 때
                printf("%2d : PUSH(1, %d)\n", count++, start[i]);
                Push(s1, start[i]);
                if(s1->top+1 > stackMax1){
                    stackMax1 = s1->top+1;
                }
                least1 = start[i];

            } else if (isEmpty(s2)) { //스택2가 비어있을 때
                printf("%2d : PUSH(2, %d)\n", count++, start[i]);
                Push(s2, start[i]);
                if(s2->top+1 > stackMax2){
                    stackMax2 = s2->top+1;
                }
                least2 = start[i];
            } else if (!isEmpty(s1) && least1 < start[i]) { //스택1이 안 비어 있는데 최솟값보다 IN이 더 크면
                printf("%2d : PUSH(2, %d)\n", count++, start[i]);
                Push(s2, start[i]);
                if(s2->top+1 > stackMax2){
                    stackMax2 = s2->top+1;
                }
            } else if (!isEmpty(s1) && least1 > start[i]) { //스택1이 안 비어 있는데 IN이 최솟값이면
                printf("%2d : PUSH(1, %d)\n", count++, start[i]);
                Push(s1, start[i]);
                if(s1->top+1 > stackMax1){
                    stackMax1 = s1->top+1;
                }
                least1 = start[i];

            }
            i++;  //입력한 다음 숫자 보기(IN)
        }
    }
    printf("종료(총 %d회)\n", --count);
    printf("최대적재량: 1번 %d대, 2번 %d대", stackMax1,stackMax2);

}