#include <stdio.h>
#include <math.h>

#define MAX(a, b) ((a)>(b)?(a):(b))
#define MAX_DEGREE 8

typedef struct
{
    int coef[MAX_DEGREE];
    int degree;

} polynomial;

polynomial getPoly(int num)
{
    int i = 0, n;
    int comparenumber = 0;
    polynomial p = { 0 };

    printf("수식 %d은 몇 개의 항으로 구성되어 있나요?: ", num);
    scanf("%d", &n);
    printf("수식 %d를 입력하세요: ", num);

    for (i = 0; i < n; i++)
    {
        int a, b;
        scanf("%d %d", &a, &b);
        p.coef[b] = a;           //구조체의 계수 인덱스(b)는 값이 a == b번 째 계수의 값은 a다

        if (comparenumber < b)
        {
            comparenumber = b;
        }
    }

    p.degree = comparenumber;       //최대차수 변수

    return p;
}

polynomial poly_add1(polynomial A, polynomial B)
{
    polynomial C;

    C.degree = MAX(A.degree, B.degree);

    for (int i = C.degree; i >= 0;i--)
    {
        C.coef[i] = A.coef[i] + B.coef[i];      //최대차수에서 시작하여 하나씩 감소하며 더함
    }
    return C;
}


polynomial poly_multi1(polynomial X, polynomial Y)
{
    polynomial D;
    int i, j;

    for (i = 0; i <= X.degree + Y.degree; i++)
    {
        D.coef[i] = 0;
    }
    D.degree = X.degree + Y.degree;

    for (i = 0; i <= X.degree; i++)
    {
        for (j = 0; j <= Y.degree; j++)
        {
            D.coef[i + j] += X.coef[i] * Y.coef[j];
        }
    }
    return D;
}

int poly_eval(polynomial p, int x)
{
    double sum = 0;


    int chosenFunction[MAX_DEGREE];
    for (int i = 0; i < MAX_DEGREE; i++)
    {
        chosenFunction[i] = p.coef[i];
    }

    for (int i = 0; i <= p.degree; i++)
    {
        sum += pow((double)x, (double)i) * chosenFunction[i];
    }

    printf("값: %d\n", (int)sum);
}

void print_poly(polynomial p)
{
    for (int i = p.degree; i > 0; i--)
    {
        printf("%dx^%d + ", p.coef[i], i);
    }
    printf("%d\n", p.coef[0]);
    printf("\n");
}


int main()
{
    polynomial a, b, c, d;
    int input;

    a = getPoly(1);
    b = getPoly(2);

    printf("수식1: ");
    print_poly(a);
    printf("수식2: ");
    print_poly(b);

    printf("덧셈: ");
    c = poly_add1(a, b);
    print_poly(c);

    printf("곱셈: ");
    d = poly_multi1(a, b);
    print_poly(d);

    int num;

    while (1)
    {
        printf("수식을 선택하세요: ");
        scanf("%d %d", &input,&num);

        switch (input) {
            case 1:
                poly_eval(a,num);
                break;
            case 2:
                poly_eval(b,num);
                break;
            case 3:
                poly_eval(c,num);
                break;
            case 4:
                poly_eval(d,num);
                break;
        }
    }
    return 0;
}