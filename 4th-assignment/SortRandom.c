#define SWAP(x, y, t) ((t) = (x), (x) = (y), (y) = (t))

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int stdnum[9];
    char name[11]; //10글자(마지막은 널문자)
    int phone[11];
} Student;

//학번에 의한 선택 정렬
void selection_sort_Bystdnum(Student *list[], int n) {
    int i, j, l, least; //least는 제일 작은 학번의 구조체 번지수
    Student *temp;

    for (i = 0; i < n - 1; i++) { //정렬되지 않은 구간의 첫 시작
        least = i;
        for (j = i + 1; j < n; j++) { //least == i이므로 i + 1부터 비교
            for (l = 0; l < 9; l++) {
                if (list[j]->stdnum[l] < list[least]->stdnum[l]) {
                    least = j;
                    break; //l반복문 나가기(다음 구조체로 넘어가기)
                } else if (list[least]->stdnum[l] < list[j]->stdnum[l])
                    break;  //least구조체가 여전히 작으면 l반복문 나가기(다음 구조체로 이동)
            }
        }
        SWAP(list[i], list[least], temp); //구조체 포인터 값 교환
    }
}

//이름에 의한 선택 정렬
void selection_sort_Byname(Student *list[], int n) {
    int i, j, l, least;
    Student *temp;

    for (i = 0; i < n - 1; i++) {
        least = i;
        for (j = i + 1; j < n; j++) {
            for (l = 0; l < 9; l++) {
                if (list[j]->name[l] < list[least]->name[l]) {
                    least = j;
                    break;
                } else if (list[least]->name[l] < list[j]->name[l])
                    break;
            }
        }
        SWAP(list[i], list[least], temp); //구조체 포인터 값 교환
    }
}

//학번에 의한 퀵 정렬**
int partition_Bystdnum(Student *list[], int left, int right) {
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

void quick_sort_Bystdnum(Student *list[], int left, int right) {
    if (left < right) {
        int q = partition_Bystdnum(list, left, right); //분할 -> 피벗보다 큰것, 작은것으로 정렬(피벗 위치 반환)
        quick_sort_Bystdnum(list, left, q - 1);
        quick_sort_Bystdnum(list, q + 1, right);
    }
}

//이름에 의한 퀵 정렬
int partition_Byname(Student *list[], int left, int right) {
    int pivot[10]; //피벗 값
    Student *temp;
    int low, high;
    int i;

    low = left;
    high = right + 1;

    for (i = 0; i < 10; i++)
        pivot[i] = list[left]->name[i];

    do {
        do {
            low++;
            for (i = 0; i < 10; i++) {
                if (list[low]->name[i] < pivot[i])
                    break;
                else if (list[low]->name[i] > pivot[i])
                    break;
            }
        } while (low < right && list[low]->name[i] < pivot[i]);
        do {
            high--;
            for (i = 0; i < 10; i++) {
                if (list[high]->name[i] > pivot[i])
                    break;
                else if (list[high]->name[i] < pivot[i])
                    break;
            }
        } while (high > left && list[high]->name[i] > pivot[i]);

        if (low < high)
            SWAP(list[low], list[high], temp);
    } while (low < high);
    SWAP(list[left], list[high], temp); //작은 범위의 제일 마지막 위치와 피벗과 교환!
    return high; //피벗 위치 반환
}

void quick_sort_Byname(Student *list[], int left, int right) {
    if (left < right) {
        int q = partition_Byname(list, left, right);
        quick_sort_Byname(list, left, q - 1);
        quick_sort_Byname(list, q + 1, right);
    }
}

//학번에 의한 힙 정렬**
void adjust_Bystdnum(Student *a[], int root, int n) {
    int rootkey[9]; //맨 꼭대기 값
    int child = root * 2; //자식 인덱스
    int i, j, check;

    for (i = 0; i < 9; i++)
        rootkey[i] = a[root]->stdnum[i];

    while (child <= n) {
        if (child < n) {
            for (i = 0; i < 9; i++)
                if (a[child]->stdnum[i] < a[child + 1]->stdnum[i]) { //오른쪽 자식과 왼쪽 자식 비교
                    child++;
                    break;
                } else if (a[child]->stdnum[i] > a[child + 1]->stdnum[i])
                    break;
        }
        check = 0;
        for (i = 0; i < 9; i++) {
            if (rootkey[i] > a[child]->stdnum[i]) {
                check = 1;
                break;  //1. 부모가 자식보다 더 크면 반복문 나가기
            } else if (rootkey[i] < a[child]->stdnum[i]) { //2. 그렇지않고 자식이 더 크면
                for (j = 0; j < 9; j++)
                    a[child / 2]->stdnum[j] = a[child]->stdnum[j]; //부모에 자식 값 복사
                child *= 2; //자식의 자식으로 내려가기
                break;
            }
        }
        if (check) // if (rootkey[i] > a[child]->stdnum[i]) -> child *= 2 때문에 배열 범위를 벗어난 a[child]를 검사하기 때문!!
            break; //1. 맨 꼭대기 값이 자식보다 더 크면 while반복문 나가기
    }
    for (i = 0; i < 9; i++)
        a[child / 2]->stdnum[i] = rootkey[i]; // 1번으로 끝남: 부모에 맨 꼭대기 값 넣기
    // 2번으로 끝남: 자식에 맨 꼭대기 값 넣기
}

void heapsort_Bystdnum(Student *a[], int n) {
    int i, j;
    Student *temp;

    for (i = n / 2; i > 0; i--)
        adjust_Bystdnum(a, i, n); //initial heap 만들기

    for (i = n - 1; i > 0; i--) {
        SWAP(a[1], a[i + 1], temp); //힙의 1번지와 제일 밑에 있는 수와 교환
        adjust_Bystdnum(a, 1, i); //가장 큰 수들을 제외한(i) 갯수
    }
}

//이름에 의한 힙 정렬
void adjust_Byname(Student *a[], int root, int n) {
    char rootkey[11]; //맨 꼭대기 값
    int child = root * 2; //자식 인덱스
    int i, j, check;

    strcpy(rootkey, a[root]->name);

    while (child <= n) {
        if (child < n) {
            for (i = 0; i < 10; i++)
                if (a[child]->name[i] < a[child + 1]->name[i]) { //오른쪽 자식과 왼쪽 자식 비교
                    child++;
                    break;
                } else if (a[child]->name[i] > a[child + 1]->name[i])
                    break;
        }
        check = 0;
        for (i = 0; i < 10; i++) {
            if (rootkey[i] > a[child]->name[i]) {
                check = 1;
                break;  //1. 부모가 자식보다 더 크면 반복문 나가기
            } else if (rootkey[i] < a[child]->name[i]) { //2. 그렇지않고 자식이 더 크면
                for (j = 0; j < 10; j++)
                    a[child / 2]->name[j] = a[child]->name[j]; //부모에 자식 값 복사
                child *= 2; //자식의 자식으로 내려가기
                break;
            }
        }
        if (check) // if (rootkey[i] > a[child]->stdnum[i]) -> child *= 2 때문에 배열 범위를 벗어난 a[child]를 검사하기 때문!!
            break; //1. 맨 꼭대기 값이 자식보다 더 크면 while반복문 나가기
    }
    strcpy(a[child / 2]->name, rootkey); // 1번으로 끝남: 부모에 맨 꼭대기 값 넣기
    // 2번으로 끝남: 자식에 맨 꼭대기 값 넣기
}

void heapsort_Byname(Student *a[], int n) {
    int i, j;
    Student *temp;

    for (i = n / 2; i > 0; i--)
        adjust_Byname(a, i, n); //initial heap 만들기

    for (i = n - 1; i > 0; i--) {
        SWAP(a[1], a[i + 1], temp); //힙의 1번지와 제일 밑에 있는 수와 교환
        adjust_Byname(a, 1, i); //가장 큰 수들을 제외한(i) 갯수
    }
}

int main() {
    Student *a[1000], *a2[1000]; //구조체를 가리키는 '포인터의 배열'(하나는 학번 기준, 다른 하나는 이름 기준)
    Student *b[1000], *b2[1000]; //퀵 정렬
    Student *c[1001], *c2[1001]; //힙 정렬(1번지부터 시작!)
    char again; //중복 검사를 언제까지 해야하는가
    clock_t start, finish; //정렬에 소요되는 시간 계산
    double duration;
    int i, j, x, y;

    for (i = 0; i < 1000; i++) {
        a[i] = malloc(sizeof(Student));
        a2[i] = a[i];
        b[i] = a[i];
        b2[i] = a[i];
        c[i + 1] = a[i];
        c2[i + 1] = a[i];
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

    printf("<선택 정렬>\n");
    start = clock();
    selection_sort_Bystdnum(a, 1000);
    finish = clock();
    duration = (double) (finish - start) / CLOCKS_PER_SEC;
    printf("1. 학번을 기준으로 정렬: %f초 걸림\n", duration);

    for (i = 0; i < 1000; i += 25) { //1000명이 너무 많아서 25명씩 띄어서 화면에 출력
        for (j = 0; j < 9; j++)
            printf("%d", a[i]->stdnum[j]);
        printf(" ");
        printf("%s ", a[i]->name);
        for (j = 0; j < 11; j++)
            printf("%d", a[i]->phone[j]);
        printf("\n");
    }

    start = clock();
    selection_sort_Byname(a2, 1000);
    finish = clock();
    duration = (double) (finish - start) / CLOCKS_PER_SEC;
    printf("2. 이름을 기준으로 정렬: %f초 걸림\n", duration);

    for (i = 0; i < 1000; i += 25) {
        for (j = 0; j < 9; j++)
            printf("%d", a2[i]->stdnum[j]);
        printf(" ");
        printf("%s ", a2[i]->name);
        for (j = 0; j < 11; j++)
            printf("%d", a2[i]->phone[j]);
        printf("\n");
    }

    printf("\n<퀵 정렬>\n");
    start = clock();
    quick_sort_Bystdnum(b, 0, 999);
    finish = clock();
    duration = (double) (finish - start) / CLOCKS_PER_SEC;
    printf("1. 학번을 기준으로 정렬: %f초 걸림\n", duration);

    for (i = 0; i < 1000; i += 25) {
        for (j = 0; j < 9; j++)
            printf("%d", b[i]->stdnum[j]);
        printf(" ");
        printf("%s ", b[i]->name);
        for (j = 0; j < 11; j++)
            printf("%d", b[i]->phone[j]);
        printf("\n");
    }

    start = clock();
    quick_sort_Byname(b2, 0, 999);
    finish = clock();
    duration = (double) (finish - start) / CLOCKS_PER_SEC;
    printf("2. 이름을 기준으로 정렬: %f초 걸림\n", duration);

    for (i = 0; i < 1000; i += 25) {
        for (j = 0; j < 9; j++)
            printf("%d", b2[i]->stdnum[j]);
        printf(" ");
        printf("%s ", b2[i]->name);
        for (j = 0; j < 11; j++)
            printf("%d", b2[i]->phone[j]);
        printf("\n");
    }

    printf("\n<힙 정렬>\n");
    start = clock();
    heapsort_Bystdnum(c, 1000);
    finish = clock();
    duration = (double) (finish - start) / CLOCKS_PER_SEC;
    printf("1. 학번을 기준으로 정렬: %f초 걸림\n", duration);

    for (i = 1; i <= 1000; i += 25) {
        for (j = 0; j < 9; j++)
            printf("%d", c[i]->stdnum[j]);
        printf(" ");
        printf("%s ", c[i]->name);
        for (j = 0; j < 11; j++)
            printf("%d", c[i]->phone[j]);
        printf("\n");
    }

    start = clock();
    heapsort_Byname(c2, 1000);
    finish = clock();
    duration = (double) (finish - start) / CLOCKS_PER_SEC;
    printf("2. 이름을 기준으로 정렬: %f초 걸림\n", duration);

    for (i = 1; i <= 1000; i += 25) {
        for (j = 0; j < 9; j++)
            printf("%d", c2[i]->stdnum[j]);
        printf(" ");
        printf("%s ", c2[i]->name);
        for (j = 0; j < 11; j++)
            printf("%d", c2[i]->phone[j]);
        printf("\n");
    }
}