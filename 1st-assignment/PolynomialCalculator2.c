#include <stdio.h>
#include <stdlib.h>

#define MAX_TERMS 101

typedef struct
{
    int coef;
    int expon;
}polynomial;

polynomial terms[MAX_TERMS] = { 0 };                    // 식1, 식2, 덧셈 결과 저장
polynomial result[MAX_TERMS] = { 0 };                   // 식1, 식2, 곱셈 결과 저장

int avail = 0;                                          // 배열의 다음 변수 인덱스


// 새로운 항 추가 함수
void attach(int coef, int expon)
{
    if (avail >= MAX_TERMS) {
        printf("다항식에 항이 너무 많다.");
        exit(1);
    }

    terms[avail].coef = coef;                           // terms에 coef값 입력
    terms[avail].expon = expon;                         // terms에 expon값 입력
    avail++;
}

// 곱셈 항 추가 함수
void multi_attach(int coef, int expon, int x)
{
    int i;
    avail--;

    for (i = avail; i >= x; i--)
    {
        if (result[i].expon == expon)                 // 같은 지수가 있는지 검사
        {
            result[i].coef += coef;                   // 같은 지수의 계수에 더하기
            avail++;
            return;
        }
    }

    avail++;
    result[avail].coef = coef;
    result[avail].expon = expon;
    avail++;
}

// 두 개의 정수를 비교
char compare(int a, int b)
{
    if (a > b)
        return '>';
    else if (a < b)
        return '<';
    else if (a == b || a == b == 0)
        return '=';

}

// 덧셈
void poly_add2(int As, int Ae, int Bs, int Be, int* Cs, int* Ce)
{
    int tempcoef;
    *Cs = avail;

    while (As <= Ae && Bs <= Be)
    {
        switch (compare(terms[As].expon, terms[Bs].expon))
        {
            case '>':
                attach(terms[As].coef, terms[As].expon);
                As++;
                break;
            case '=':
                tempcoef = terms[As].coef + terms[Bs].coef;
                if (tempcoef)
                    attach(tempcoef, terms[As].expon);
                As++;
                Bs++;
                break;
            case '<':
                attach(terms[Bs].coef, terms[Bs].expon);
                Bs++;
                break;
        }
    }

    // A의 나머지 항들을 이동함
    for (;As <= Ae;As++)
    {
        attach(terms[As].coef, terms[As].expon);
    }

    // B의 나머지 항들을 이동함
    for (;Bs <= Be;Bs++)
    {
        attach(terms[Bs].coef, terms[Bs].expon);
    }

    *Ce = avail - 1;
}

// 곱셈
void poly_multi2(int As, int Ae, int Bs, int Be, int* Ds, int* De)
{

    int coef;
    int expon;
    int i, j;
    int crit = Ae + Be;
    *Ds = avail;

    for (i = As;i <= Ae;i++)
    {
        for (j = Bs;j <= Be;j++)
        {
            coef = result[i].coef * result[j].coef;
            expon = result[i].expon + result[j].expon;
            multi_attach(coef, expon, crit);
        }
    }

    result[avail].coef = result[Ae].coef * result[Be].coef;
    result[avail].expon = result[Ae].expon + result[Be].expon;
    avail++;

    *De = avail - 1;
}

// terms 배열 계산
int poly_eval(int start, int finish, int x)
{
    int i, expon;
    int temp[MAX_TERMS] = { 0 };                            // 계산할 식을 복사하기 위한 배열                          
    int sum = 0;

    if (start <= finish)
    {
        for (i = start; i < finish; i++)                    // 마지막 항을 제외한 항들의 계수를 복사
            temp[i] = terms[i].coef;

        for (i = start; i < finish; i++)
        {
            for (int j = 0; j < terms[i].expon; j++)        // expon만큼 입력한 수를 곱한 후 temp에 저장
                temp[i] *= x;
        }

        // 마지막 항을 출력 
        if (terms[finish].expon != 0)                       // 마지막 항의 차수가 0이 아닐 때
        {
            terms[finish].coef *= x;
        }
        else {
            for (int j = 0; j < terms[finish].expon; j++)   // 마지막 항의 차수가 0일 때
                temp[finish] *= x;
        }

        for (i = start; i <= finish; i++)
            sum += temp[i];
        sum += terms[finish].coef;

        printf("sum = %d\n ", sum);
    }

    else
        printf(" No terms ");
}

// 곱셈 수식 계산(result)
int poly_evalMulti(int start, int finish, int x)
{
    int i, expon, temp[MAX_TERMS] = { 0 };
    int sum = 0;

    if (start <= finish)
    {
        for (i = start; i < finish; i++)
            temp[i] = result[i].coef;
        for (i = start; i < finish; i++)
        {
            //temp*=x; 
            for (int j = 0; j < result[i].expon; j++)
                temp[i] *= x;
        }
        // 마지막 항을 출력 
        if (result[finish].expon != 0)
        {
            result[finish].coef *= x;
        }
        else {
            for (int j = 0; j < result[finish].expon; j++)
                temp[finish] *= x;
        }

        for (i = start; i <= finish; i++)
            sum += temp[i];
        sum += result[finish].coef;
        printf("sum = %d\n ", sum);
    }
    else
        printf(" No terms ");
}

// 출력
void print_poly(int s, int e)
{
    int i;

    for (i = s; i < e; i++)
    {
        printf("%d x^ %d + ", terms[i].coef, terms[i].expon);
    }
    printf("%d x^ %d \n", terms[e].coef, terms[e].expon);
}

void print_multi(int s, int e)
{
    int i;

    for (i = s; i < e; i++) {
        printf("%d x^ %d + ", result[i].coef, result[i].expon);
    }
    printf("%d x^ %d \n", result[e].coef, result[e].expon);
}

int main() {

    int As, Ae; // 다항식 1
    int Bs, Be; // 다항식 2
    int Cs, Ce; // 덧셈
    int Ds, De; // 곱셈

    int i, n;
    int x, y;
    int input;
    polynomial a, b, c, d;
    printf("수식 1은 몇 개의 항으로 구성되어 있나요? ");
    scanf("%d", &n);

    As = avail;
    printf("수식 1을 입력하세요: ");
    for (i = 0; i < n; i++) {
        scanf("%d %d", &x, &y);
        attach(x, y);
    }
    Ae = avail - 1;
    printf("\n");

    printf("수식1: ");
    print_poly(As, Ae);
    printf("\n");

    printf("수식 2는 몇 개의 항으로 구성되어 있나요? ");
    scanf("%d", &n);

    Bs = avail;
    printf("수식 2를 입력하세요: ");
    for (i = 0;i < n;i++) {
        scanf("%d %d", &x, &y);

        attach(x, y);
    }
    Be = avail - 1;
    printf("\n");

    printf("수식2: ");
    print_poly(Bs, Be);
    printf("\n");

    for (i = 0;i <= Be;i++) {
        result[i].expon = terms[i].expon;
        result[i].coef = terms[i].coef;
    }

    poly_add2(As, Ae, Bs, Be, &Cs, &Ce);

    printf("덧셈: ");
    print_poly(Cs, Ce);
    printf("\n");

    poly_multi2(As, Ae, Bs, Be, &Ds, &De);
    printf("곱셈: ");
    print_multi(Ds, De);
    printf("\n");
    int num;


    while (1)
    {
        printf("수식을 선택하세요: ");
        scanf("%d %d", &num, &input);

        switch (num) {
            case 1:
                poly_eval(As, Ae, input);
                break;
            case 2:
                poly_eval(Bs, Be, input);
                break;
            case 3:
                poly_eval(Cs, Ce, input);
                break;
            case 4:
                poly_evalMulti(Ds, De, input);
                break;
        }
    }

    return 0;

}