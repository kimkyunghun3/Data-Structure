#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK 150
typedef unsigned char element;

typedef struct {
    int top;
    element data[MAX_STACK];
}stack;

void init_stack(stack* s) {
    s->top = -1;          // 스택 초기화: top -1로 설정
}

int is_empty(stack* s) {

    return (s->top == -1);    // 비어있으면 -1 리턴
}

void push(stack* s, element input) // 스택에 데이터 추가 함수
{
    s->data[++(s->top)] = input;
}

element pop(stack* s) {       // 스택에 데이터 제거 함수

    if (is_empty(s)) {
        printf("stack is empty\n");

    }
    else {
        return(s->data[(s->top)--]);
    }
}

element peek(stack* s) {

    if (is_empty(s)) {
        printf("stack is empty\n");

    }
    else {
        return (s->data[s->top]);
    }
}

//우선순위 반환 함수
int operator(char op) {

    switch (op) {
        case '(':
        case ')':
            return 0;
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
    }
    return -1;      // 연산자가 아니면 -1 리턴
}

// 중위 -> 후위표기식
char* infix_postfix(char* s) {

    int i;
    int len = strlen(s);
    char temp;
    int top;
    char* post = (char*)malloc(sizeof(char) * len);      // 후위표기식을 저장하는 배열 , 동적할당으로 선언
    int idx = 0;   // 후위표기식 인덱스 값을 위한 변수
    stack b;      // 연산자를 넣기 위한 스택 선언
    init_stack(&b);

    for (i = 0; i < len;i++) {
        temp = s[i];
        switch (temp) {
            case '(':
                push(&b, temp);
                break;
            case ')':
                top = pop(&b);
                while (top != '(') {
                    post[idx++] = ' ';
                    post[idx++] = top;
                    top = pop(&b);
                }
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
                while (!is_empty(&b) && operator(temp) <= operator(peek(&b))) {
                    post[idx++] = ' ';
                    post[idx++] = pop(&b);
                }
                push(&b, temp);
                post[idx++] = ' ';
                break;
            default:
                post[idx++] = temp;
                break;
        }//switch
    }//for
    while (!is_empty(&b)) {
        post[idx++] = ' ';
        post[idx++] = pop(&b);
    }
    post[idx] = '\0';
    return post;
}

// 후위 표기 연산
int calculate(char* s) {

    int i, j;
    int len = strlen(s);
    int power = 1;      // 제곱근 결과 저장
    // int result = 0 ;         // 결과 저장
    int op1, op2;
    int symb;

    stack c;
    init_stack(&c);

    for (i = 0; i < len; i++) {

        char temp[3] = { 0, };
        int idx = 0;
        int result = 0;
        while (1)
        {
            if ((s[i] == ' ') || (s[i] == '\0'))
            {
                break;
            } //if

            temp[idx] = s[i];
            i++;
            idx++;
        }

        if (operator(temp[0])>=1) {

            op2 = pop(&c);   // 피연산자 pop
            op1 = pop(&c);   // 피연산자 pop

            switch (temp[0])   // 실제 계산이 이루어짐
            {
                case '*':
                    result = op1 * op2;
                    break;
                case '/':
                    result = op1 / op2;
                    break;
                case '+':
                    result = op1 + op2;
                    break;
                case '-':
                    result = op1 - op2;
                    break;
                case '^':   // 제곱일 때
                    for (j = 0; j < op2; j++)   // op2 만큼 반복
                    {
                        power *= op1;   // op1을  op2만큼 계속해서 곱해줌
                    }
                    result = power;
                    break;
            }
            push(&c, result);
        }

        else {
            symb = atoi(temp);
            push(&c, symb);
        }
    }

    return pop(&c);
}

// 문장 검사 함수
int check_infix(char* s) {

    int len = strlen(s);
    int i;
    char temp;
    char check;
    stack a;
    init_stack(&a);

    for (i = 0;i < len;i++) {
        temp = s[i];

        switch (temp) {
            case '(':
                push(&a, temp);
                break;
            case ')':
                if (is_empty(&a)) {
                    printf("%s(i) 이 위치에 오류가 있습니다.\n", s);
                    return -1;
                }
                pop(&a);
                break;

            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
                check = s[i + 1];
                if (check == '+' || check == '-' || check == '*' || check == '/' || check == '^') {
                    printf("%s 이 위치에 오류가 있습니다. 숫자가 와야 합니다\n",s);


                    return -1;
                }//if
                break;
        }//switch
    }

    // 모든 검사가 끝났을 때 stack a 가 비어있지 않음
    if (!is_empty(&a)) {
        printf("%s(!) 이 위치에 오류가 있습니다. )가 부족합니다\n",s);
        return -1;
    }

    return 0;
}

int main() {

    char s[100];   // 중위표기식 입력
    char* post;      // 후위표기식 입력
    int t;
    int result = 0;

    printf("식 입력: ");
    scanf("%s", &s);
    t = check_infix(s);            // 중위표기식 검사
    char* p = infix_postfix(s);      // 후위표기식


    if (t == 0) {

        printf("중위 : %s\n", s);
        printf("후위 : %s\n", p);
        result = calculate(p);
        printf("결과 값: %d ",result);
    }
    else {
        exit(1);
    }

    return 0;
}