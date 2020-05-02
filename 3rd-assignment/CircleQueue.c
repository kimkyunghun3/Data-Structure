#include <stdio.h>

#define MAX_QUEUE_SIZE 20
#define element int

#include <string.h>
#include <stdlib.h>

#define Success 1
#define Error -1

typedef struct {
    int queue[MAX_QUEUE_SIZE];
    int front;
    int rear;

} QueueType;

//큐 생성
QueueType *create() {
    QueueType *tempqueue;

    tempqueue = malloc(sizeof(QueueType));
    tempqueue->front = 0;
    tempqueue->rear = 0;
    return tempqueue;
};

//공백 상태 검출 함수
int is_empty(QueueType *q) {
    return (q->front == q->rear);
}

//포화 상태 검출 함수
int is_full(QueueType *q) {

    return (q->rear + 1 % MAX_QUEUE_SIZE == q->front);
}

//삽입 함수
int enqueue(QueueType *q, element item) {
    if (is_full(q))
        return Error;
    else {
        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
        q->queue[q->rear] = item;
        return Success;
    }
}

//삭제 함수
element dequeue(QueueType *q) {
    if (is_empty(q))
        return Error;
    else {
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;

        return q->queue[q->front];
    }

}

//출력 함수
void queue_print(QueueType *q) {
    int i;
    if (!is_empty(q)) {
        i = q->front;
        do {
            i = (i + 1) % MAX_QUEUE_SIZE;
            printf("%c", q->queue[i]);
            if (i == q->rear)
                break;
        } while (i != q->front);
    }

}

void main() {
    char user[25];
    QueueType *q;
    int i;
    int count = 0;
    int tempnum;
    char strDe[20] = {'\0'};
    int index = 0;

    q = create();
    printf(">>>");
    scanf("%s", user);

    while (strcmp(user, "stop") != 0) {
        if (user[0] >= 'A') {
            for (i = 0; i < strlen(user); i++) {
                printf("ADDQUEUE(%c) ", user[i]);
                enqueue(q, user[i]);

                if (count > 20)
                    printf("Fail : Queue Full ");
                else
                    printf("F=%d R=%d \n", q->front, q->rear);

            }


        } else if (user[0] >= '0' && user[0] <= '9') {
            index = 0;
            if (user[1] >= '0' && user[1] <= '9')
                tempnum = (user[0] - '0') * 10 + user[1] - '0';
            else
                tempnum = user[0] - '0';
            for (i = 0; i < tempnum; i++) {
                strDe[index] = dequeue(q);
                printf("DELETEQUEUE() = %c, F=%d, R= %d\n", strDe[index++], q->front, q->rear);

            }
            printf("RESULT = %s \n", strDe);
        }
        printf("QUEUE = ");
        queue_print(q);
        printf("(%d)", (q->rear) - (q->front));
        printf("\n");
        for (i = 0; i < 20; i++) {
            if (strDe[i] == '\0')
                break;
            strDe[i] = '\0';
        }

        printf(">>>");
        scanf("%s", user);
    }
}
