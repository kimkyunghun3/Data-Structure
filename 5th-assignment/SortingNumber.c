#define MAX_NUM 10000
#include <stdio.h>
#include <stdlib.h>

typedef struct DlistNode {
    int id;
    char name[11];
    char phone[12];
    struct DlistNode* link;
    struct DlistNode* id_link;
    struct DlistNode* name_link;
}DlistNode;

//이중 연결리스트 삽입 함수(새로운 노드 리스트 맨 처음에 삽입 -> 이름,학번 링크 연결)
void insert_node(DlistNode* head, DlistNode* new_node) {
    DlistNode* pre; //새로운 노드의 이전 노드
    DlistNode* next; //새로운 노드의 이후 노드

    new_node->link = head->link;
    head->link = new_node;
    //학번 링크 연결
    pre = head;
    next = pre->id_link;
    if (next == NULL) { //**리스트가 비어있는 경우
        new_node->id_link = NULL;
        head->id_link = new_node;
        new_node->name_link = NULL;
        head->name_link = new_node;
    }
    else {
        while (next != NULL) {
            if (new_node->id < next->id) {
                new_node->id_link = next;
                pre->id_link = new_node;
                break;
            }
            else if (next->id_link == NULL) { //**새로운 노드를 마지막에 연결
                new_node->id_link = NULL;
                next->id_link = new_node;
                break;
            }
            else { //삽입할 곳이 아니면 이동하기
                pre = pre->id_link;
                next = next->id_link;
            }
        }
        //이름 링크 연결
        pre = head;
        next = pre->name_link;
        while (next != NULL) {
            if (strcmp(new_node->name, next->name) < 0) {
                new_node->name_link = next;
                pre->name_link = new_node;
                break;
            }
            else if (next->name_link == NULL) {
                new_node->name_link = NULL;
                next->name_link = new_node;
                break;
            }
            else {
                pre = pre->name_link;
                next = next->name_link;
            }
        }
    }
}

//이중 연결리스트 출력 함수
void display(DlistNode* head, int choice) {
    DlistNode* p = head;
    int count = 1;
    //학번 순으로 출력하기
    if (choice == 1) {
        p = p->id_link;
        while (p != NULL) {
            printf("%5d.%d %s %s\n",count++, p->id, p->name, p->phone);
            p = p->id_link;
        }
    }
    else if (choice == 2) {
        p = p->name_link;
        while (p != NULL) {
            printf("%5d.%d %s %s\n",count++, p->id, p->name, p->phone);
            p = p->name_link;
        }
    }
}
//이중 연결리스트 탐색 함수(해당 학번 찾기)
DlistNode* search_id(DlistNode* head, int id) {
    DlistNode* p = head->id_link; //헤드 노드부터 탐색 시작
    while (p != NULL) {
        if (p->id == id) return p; //비교 성공
        p = p->id_link;
    }
    return p; //탐색 실패 -> NULL 반환
}
//이중 연결리스트 탐색 함수(해당 이름,전화번호 찾기)
DlistNode* search_namephone(DlistNode* head, char* data) {
    DlistNode* p = head->name_link; //헤드 노드부터 탐색 시작
    while (p != NULL) {
        if (!strcmp(p->name, data)) return p; //이름 비교 성공
        else if (!strcmp(p->phone, data)) return p; //전화번호 비교 성공
        p = p->name_link;
    }
    return p; //탐색 실패 -> NULL 반환
}

int main() {
    //더미 노드(헤드) 생성
    DlistNode* head = (DlistNode*)malloc(sizeof(DlistNode));
    head->link = NULL; //연결리스트 초기화
    head->id_link = NULL; //학번 연결리스트, 이름 연결리스트 초기화
    head->name_link = NULL;
    head->id = 0; //헤드 노드는 빈 공간
    head->name[0] = NULL;
    head->phone[0] = NULL;

    DlistNode* new_node;
    int year;
    int	idnum1, idnum2;
    int	phone1, phone2;
    int i,j;

    //1만 명분의 랜덤 데이터 생성
    srand(time(NULL));
    for (i = 0;i < MAX_NUM;i+=100) {
        new_node = (DlistNode*)malloc(sizeof(DlistNode));
        //학번 데이터
        year = rand() % 7 + 2013;
        idnum1 = rand() % 1000;
        idnum2 = rand() % 100;
        new_node->id = year * 100000 + idnum1 * 100 + idnum2;
        //이름 데이터
        for (j = 0;j < 10;j++)
            new_node->name[j] = rand() % 26 + 'A';
        new_node->name[j] = NULL;
        //전화번호 데이터
        phone1 = rand() % 10000;
        phone2 = rand() % 10000;
        sprintf(new_node->phone, "010%04d%04d", phone1, phone2);

        //중복 검사
        if (search_id(head, new_node->id) != NULL)
            i--;
        else if (search_namephone(head, new_node->name) != NULL)
            i--;
        else if (search_namephone(head, new_node->phone) != NULL)
            i--;
        else
            insert_node(head, new_node); //중복된 것이 없으면 삽입하기
    }

    int user;
    printf("(1)학번순\n(2)이름순\n메뉴 선택(0: 종료) : ");
    scanf("%d", &user);
    while (user != 0) {
        display(head, user);
        printf("\n(1)학번순\n(2)이름순\n메뉴 선택(0: 종료) : ");
        scanf("%d", &user);
    }
}