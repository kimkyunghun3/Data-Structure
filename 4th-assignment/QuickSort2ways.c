#define SWAP(x, y, t) ((t) = (x), (x) = (y), (y) = (t))
#define MAX_STACK_SIZE 50
#define True 1
#define False 0
#define Error -1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int stdnum[9];
    char name[11];
    int phone[11];
} Student;

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
        printf("스택이 다 찼다!\n");
        exit(-1); //프로그램 종료
    }
}

//스택에서 끄집어내고 없애기
Element Pop(Stack *pstack) {
    if (!isEmpty(pstack))
        return pstack->stackArr[pstack->top--];
    else {
        printf("스택이 비어있다!\n");
        return Error;  //main함수에 오류 알린다.
    }
}

//재귀호출 퀵 정렬
int partition(Student *list[], int left, int right) {
    int pivot[9]; //피벗 값
    Student *temp;
    int low, high;  //피벗 보다 작은 범위 인덱스, 피벗보다 큰 범위 인덱스
    int i;

    low = left;
    high = right + 1;

    for (i = 0; i < 9; i++)
        pivot[i] = list[left]->stdnum[i]; //피벗은 배열의 맨 처음 값

    do {
        do {
            low++;
            for (i = 0; i < 9; i++) {  //low와 피벗이 같으면 i++(다음 숫자로 이동)
                if (list[low]->stdnum[i] < pivot[i])  //low의 학번이 피벗보다 한 숫자라도 작으면
                    break; //while문으로
                else if (list[low]->stdnum[i] > pivot[i]) //low의 학번 중 한 숫자가 피벗보다 크면
                    break;
            }
        } while (low < right && list[low]->stdnum[i] < pivot[i]); //low가 배열을 넘어가지 않게 막기
        do {
            high--;
            for (i = 0; i < 9; i++) {
                if (list[high]->stdnum[i] > pivot[i])
                    break;
                else if (list[high]->stdnum[i] < pivot[i])
                    break;
            }
        } while (high > left && list[high]->stdnum[i] > pivot[i]);

        if (low < high)
            SWAP(list[low], list[high], temp);
    } while (low < high);
    SWAP(list[left], list[high], temp); //작은 범위의 제일 마지막 위치와 피벗과 교환!
    return high; //피벗 위치 반환
}

void quick_sort(Student *list[], int left, int right) {
    if (left < right) {
        int q = partition(list, left, right); //분할 -> 피벗보다 큰것, 작은것으로 정렬(피벗 위치 반환)
        quick_sort(list, left, q - 1);
        quick_sort(list, q + 1, right);
    }
}

//스택 퀵 정렬
int partition_stack(Student *list[], int left, int right) {
    int pivot[9];
    Student *temp;
    int low, high;
    int i;

    low = left;
    high = right + 1;

    for (i = 0; i < 9; i++)
        pivot[i] = list[left]->stdnum[i];

    do {
        do {
            low++;
            for (i = 0; i < 9; i++) {
                if (list[low]->stdnum[i] < pivot[i])
                    break;
                else if (list[low]->stdnum[i] > pivot[i])
                    break;
            }
        } while (low < right && list[low]->stdnum[i] < pivot[i]);
        do {
            high--;
            for (i = 0; i < 9; i++) {
                if (list[high]->stdnum[i] > pivot[i])
                    break;
                else if (list[high]->stdnum[i] < pivot[i])
                    break;
            }
        } while (high > left && list[high]->stdnum[i] > pivot[i]);

        if (low < high)
            SWAP(list[low], list[high], temp);
    } while (low < high);
    SWAP(list[left], list[high], temp);
    return high;
}

void quick_sort_stack(Student *list[], int left, int right) {
    Stack *s;
    int q, l, r;
    s = Create();
    Push(s, left);
    Push(s, right);
    while (!isEmpty(s)) {
        r = Pop(s);
        l = Pop(s);
        if (0 <= l && l < r) {
            q = partition_stack(list, l, r);
            Push(s, l);
            Push(s, q - 1);
            Push(s, q + 1);
            Push(s, r);
        }
    }
}

int main() {
    Student *a[1000], *a2[1000]; //구조체를 가리키는 '포인터의 배열'(하나는 재귀 퀵 정렬, 다른 하나는 스택 퀵 정렬)
    char again; //중복 검사를 언제까지 해야하는가
    clock_t start, finish; //정렬에 소요되는 시간 계산
    double duration;
    int i, j, x, y;

    for (i = 0; i < 1000; i++) {
        a[i] = malloc(sizeof(Student));
        a2[i] = a[i];
    }
    srand(time(NULL));
    //1000명의 랜덤 데이터 생성
    for (i = 0; i < 1000; i++) {
        again = 1;
        while (again) {  //<학번>
            a[i]->stdnum[0] = 2;
            a[i]->stdnum[1] = 0;
            a[i]->stdnum[2] = 1;
            a[i]->stdnum[3] = (rand() % 7) + 3;
            for (j = 4; j < 9; j++)
                a[i]->stdnum[j] = rand() % 10;

            again = 0;
            for (x = 0; x < i; x++) { //중복 데이터 검사
                for (y = 0; y < 9; y++) {
                    if (a[x]->stdnum[y] != a[i]->stdnum[y])  //x번째 구조체에서 한 숫자라도 다르면
                        break; //y반복문 나가기
                }
                if (y == 10) { //x번째 구조체가 현재 구조체와 같다면
                    again = 1; //다시 학번 정하기
                    break; //x반복문을 나가라
                } //서로 다르다면 다음 구조체 검사
            }
        }//학번
        again = 1;
        a[i]->name[10] = NULL; //문자열 초기화
        while (again) {  //<이름>
            for (j = 0; j < 10; j++)
                a[i]->name[j] = (rand() % 26) + 65; //ascii code

            again = 0;
            for (x = 0; x < i; x++) { //'중복 데이터 검사'
                if (strcmp(a[x]->name, a[i]->name) == 0) { //x번째 구조체 학번과 같으면 ****
                    again = 1; //다시 학번 정하기
                    break; //x반복문 나가기
                }
            }
        }//이름
        again = 1;
        while (again) {  //<전화번호>
            a[i]->phone[0] = 0;
            a[i]->phone[1] = 1;
            a[i]->phone[2] = 0;
            for (j = 3; j < 11; j++)
                a[i]->phone[j] = rand() % 10;

            again = 0;
            for (x = 0; x < i; x++) { //중복 데이터 검사
                for (y = 0; y < 11; y++) {
                    if (a[x]->phone[y] != a[i]->phone[y])
                        break;
                }
                if (y == 10) {
                    again = 1;
                    break;
                }
            }
        }//전화번호
    }//랜덤 데이터 생성

    printf("<재귀 호출 퀵 정렬>\n");
    start = clock();
    quick_sort(a, 0, 999);
    finish = clock();
    duration = (double) (finish - start) / CLOCKS_PER_SEC;
    printf("학번을 기준으로 정렬: %f초 걸림\n", duration);

    for (i = 0; i < 1000; i += 25) {
        for (j = 0; j < 9; j++)
            printf("%d", a[i]->stdnum[j]);
        printf(" ");
        printf("%s ", a[i]->name);
        for (j = 0; j < 11; j++)
            printf("%d", a[i]->phone[j]);
        printf("\n");
    }

    printf("\n<스택 퀵 정렬>\n");
    start = clock();
    quick_sort_stack(a2, 0, 999);
    finish = clock();
    duration = (double) (finish - start) / CLOCKS_PER_SEC;
    printf("학번을 기준으로 정렬: %f초 걸림\n", duration);

    for (i = 0; i < 1000; i += 25) {
        for (j = 0; j < 9; j++)
            printf("%d", a2[i]->stdnum[j]);
        printf(" ");
        printf("%s ", a2[i]->name);
        for (j = 0; j < 11; j++)
            printf("%d", a2[i]->phone[j]);
        printf("\n");
    }
}