#include <stdio.h>
#include <stdlib.h>

#define MAX_WORD 70000
#define MOD 70000

//해쉬 테이블의 데이터
typedef struct {
    char word[50];
    char mean[100];
} Word;
//해시 테이블, overflow -> 체이닝
typedef struct ListNode {
    Word data;
    struct ListNode *link;
} ListNode;

ListNode *hash[MOD]; //해시 테이블
int hash_count; //해시 테이블 탐색 빈도

//연결리스트 삽입 함수 - 체이닝
void insert_node(ListNode **phead, ListNode *p, ListNode *new_node) {
    if (*phead == NULL) { //비어있는 연결리스트일 경우
        new_node->link = NULL;
        *phead = new_node;
    } else if (p == NULL) { //p가 NULL이면 첫 칸에 삽입할 경우
        new_node->link = *phead;
        *phead = new_node;
    } else {  //p(previous) 다음에 삽입
        new_node->link = p->link;
        p->link = new_node;
    }
}

//해쉬 함수 - 키 주소 리턴 (이차 조사법 변형)
int hash_function(char *word) {
    int key_add = 0;
    int mult = 1;
    for (int i = 0; word[i] != NULL; i++) {
        key_add += (word[i] * mult + (i + 1) * (i + 1)) % MOD;
        mult *= 7;
    }
    if (key_add < 0) key_add = -key_add;  //절대값
    return key_add % MOD;
}

//다 꽉 찼을때 = overflow -> 체이닝
void chaining(int key, char *word, char *mean) {
    ListNode *new_node = NULL;
    new_node = (ListNode *) malloc(sizeof(ListNode));
    new_node->link = NULL;
    strcpy(new_node->data.word, word);
    strcpy(new_node->data.mean, mean);
    insert_node(&hash[key], hash[key], new_node); //체이닝 -> 해시테이블 다음에 연결하기
}

//해시 테이블에 집어넣기
void hash_add(int key, char *word, char *mean) {
    if (!strcmp(hash[key]->data.word, "")) { //해시 테이블이 비어 있다면
        strcpy(hash[key]->data.word, word);
        strcpy(hash[key]->data.mean, mean);
        return;
    } else { //충돌 발생 - 선형조사법(약간)
        for (int i = 1; i < MOD / 7; i++) {
            if (!strcmp(hash[(key + i) % MOD]->data.word, "")) { //빈칸을 찾음
                strcpy(hash[(key + i) % MOD]->data.word, word);
                strcpy(hash[(key + i) % MOD]->data.mean, mean);
                return;
            }
        }
        //꽉 찬 경우 -> 체이닝
        chaining(key, word, mean);
    }
}

//해시 테이블 탐색 - 해당 단어가 있는 곳의 의미 출력
char *hash_search(int key, char *key_word, int *count) {
    ListNode *search = NULL;
    *count = 1;

    if (!strcmp(hash[key]->data.word, "")) {
        printf("단어를 다시 입력하시오.\n");
        return NULL;
    } else {
        //1. 바로 단어 찾음
        if (!strcmp(hash[key]->data.word, key_word))
            return hash[key]->data.mean;
        // 체이닝 연결 리스트 탐색 - 처음 key노드에 링크가 있다면
        if (hash[key]->link != NULL) {
            search = hash[key]->link;
            (*count)++;
            while (search) {
                if (!strcmp(search->data.word, key_word))
                    return search->data.mean;
                search = search->link;
                (*count)++;
            }
        }
        //2. 선형조사법으로 탐색
        int i = 1;
        (*count)++;
        while (strcmp(hash[(key + i) % MOD]->data.word, hash[key]->data.word) != 0) { //다시 같은 자리로 되돌아오지않을때 까지
            if (!strcmp(hash[(key + i) % MOD]->data.word, key_word))
                return hash[(key + i) % MOD]->data.mean;
            // 체이닝 연결 리스트 탐색 - 이동한 노드에 링크가 있다면
            if (hash[(key + i) % MOD]->link != NULL) {
                search = hash[(key + i) % MOD]->link;
                (*count)++;
                while (search) {
                    if (!strcmp(search->data.word, key_word))
                        return search->data.mean;
                    search = search->link;
                    (*count)++;
                }
            }
            i++; //링크에도 없었으면 다음 테이블로 이동
            (*count)++;
        }
        printf("탐색 오류\n");
        return NULL;
    }
}

int main() {
    FILE *f;

    char str[200]; //파일의 한 줄 문장
    char data_word[30]; //한 줄 문장의 단어
    char data_mean[100]; //한 줄 문장의 의미
    char *tok;
    int key;
    int i;

    //해싱 테이블 초기화 -> 삽입이 어느곳에 될지 모르므로
    for (i = 0; i < MOD; i++) {
        hash[i] = (ListNode *) malloc(sizeof(ListNode));
        hash[i]->link = NULL;
        hash[i]->data.mean[0] = NULL;
        hash[i]->data.word[0] = NULL;
    }


    f = fopen("mod_dict_utf8.txt", "r");
    if (f != NULL) {
        for (i = 0; i < MAX_WORD; i++) {
            fgets(str, sizeof(str), f);
            tok = strtok(str, " :");
            strcpy(data_word, tok); //단어
            tok = strtok(NULL, ":\n");
            strcpy(data_mean, tok); //의미
            //키 주소 얻기 -> 해시 테이블에 집어넣기
            key = hash_function(data_word); //키 = 단어
            hash_add(key, data_word, data_mean);
        }
    } else {
        printf("파일 찾지 못함\n");
        exit(-1);
    }
    fclose(f);
    printf("단어:");
    gets(str);
    key = hash_function(str);
    printf("%s ", hash_search(key, str, &hash_count));
    printf("(%d)\n", hash_count);
}