#include <stdio.h>
#include <limits.h>
#define MAX_VERTEX 20
#define TRUE 1
#define FALSE 0
#define INF 1000 //무한대 거리

//인접 행렬로 그래프 구현
typedef struct {
    int n; //정점 개수
    int adj_mat[MAX_VERTEX][MAX_VERTEX];  //인접 행렬
    char metro[MAX_VERTEX][15];  //정점의 값(지하철 역)
    int visited[MAX_VERTEX];     //방문한 정점 표시 - 깊이 우선 탐색
}GraphType;
//그래프 초기화
void InitGraph(GraphType* g) {
    g->n = 0; //정점 개수 = 0
    for (int i = 0;i < MAX_VERTEX;i++) {
        g->metro[i][0] = NULL;  //null로 초기화
        g->visited[i] = FALSE;

        for (int j = 0; j < MAX_VERTEX; j++)
            g->adj_mat[i][j] = 0;  //연결되어야 '1'
    }
}

void InsertEdge(GraphType * g, int start, int end) {
    if (start >= g->n || end >= g->n) {
        printf("잘못된 번지입니다.\n");
        exit(-1);
    }
    g->adj_mat[start][end] = 1; //링크 연결
    g->adj_mat[end][start] = 1;
}

float weight[MAX_VERTEX][MAX_VERTEX] = {
        {0,1.1,1.2,1.3,1.4,INF,INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {1.1,0,INF, INF, INF, 1.5,1.6,INF,1.7,INF,INF,INF,INF,INF,INF,INF,INF,INF,INF,INF },
        {1.2,INF, 0,INF, INF, INF, INF, INF, 1.8,INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {1.3,INF, INF, 0,1.9,INF, INF, INF, INF, 2.1,INF, INF, INF, 2.2,INF, INF, INF, INF, INF, INF},
        {1.4,INF, INF, 1.9,0, INF, INF, INF, INF, INF, 1.66,2.0,INF, INF, INF, INF, INF, INF, INF, INF},
        { INF, 1.5,INF, INF, INF, 0,INF, 1.23,INF, INF, INF, INF, 1.55,INF, INF, INF, INF, 1.98,INF, INF},
        { INF, 1.6,INF, INF, INF, INF, 0,INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 1.12,INF, INF},
        { INF, INF, INF, INF, INF, 1.23,INF, 0,INF, INF, INF, INF, 1.69,INF, INF, INF, INF, INF, INF, INF},
        { INF, 1.7,1.8,INF, INF, INF, INF, INF, 0,INF, INF, INF, INF, 2.23,INF, INF, INF, INF, INF, INF},
        { INF, INF, INF, 2.1,INF, INF, INF, INF, INF, 0,1.72,INF, INF, INF, 2.3,INF, INF, INF, INF, INF},
        { INF, INF, INF, INF, 1.66,INF, INF, INF, INF, 1.72,0,2.34,INF, INF, INF, 2.4,INF, INF, INF, INF},
        { INF, INF, INF, INF, 2.0,INF, INF, INF, INF, INF, 2.34,0,INF, INF, INF, INF, 2.5,INF, INF, INF},
        { INF, INF, INF, INF, INF, 1.55,INF, 1.69,INF, INF, INF, INF, 0,1.67,INF, INF, INF, 1.37,INF, INF},
        { INF, INF, INF, 2.2,INF, INF, INF, INF, 2.23,INF, INF, INF, 1.67,0,2.8,INF, INF, INF, 1.85,INF},
        { INF, INF, INF, INF, INF, INF, INF, INF, INF, 2.3,INF, INF, INF, 2.8,0,INF, INF, INF, 2.9,2.7},
        { INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 2.4,INF, INF, INF, INF, 0,2.6,INF, INF, INF},
        { INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 2.5,INF, INF, INF, 2.6,0,INF, INF, INF},
        { INF, INF, INF, INF, 1.98,1.12,INF, INF, INF, INF, INF, 1.37,INF, INF, INF, INF, 0,1.44,INF},
        { INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 1.85,2.9,INF, INF, 1.44,0,1.99},
        { INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 2.7,INF, INF, INF, 1.99,0} };

float distance[MAX_VERTEX][2]; //최단경로, 몇번지 경유점 저장
int found[MAX_VERTEX];    //방문한 정점들

//최단경로 알고리즘의 최단 거리 함수
int choose(float distance[][2], int n, int found[]) {
    int i, minpos;
    float min;
    min = INT_MAX;
    minpos = -1;
    for (i = 0;i < n;i++) {
        if (distance[i][0] < min && !found[i]) {
            min = distance[i][0];
            minpos = i;
        }
    }
    return minpos;
}
//최단 경로 찾기 알고리즘
void shortest_path(int start, int n) {
    int i, u, w;
    for (i = 0;i < n;i++) { //초기화
        distance[i][0] = weight[start][i];
        if(distance[i][0] != INF)
            distance[i][1] = start;
        found[i] = FALSE;
    }
    found[start] = TRUE; //시작점 방문 표시
    distance[start][0] = 0;

    for (i = 0;i < n - 2;i++) {
        u = choose(distance, n, found); //방문하지 않은 곳 중 최단 거리인 정점u
        found[u] = TRUE;  //정점u를 S집합에 포함
        for (w = 0;w < n;w++) //distance 업데이트
            if (!found[w])
                if (distance[u][0] + weight[u][w] < distance[w][0]) { // start에서 출발해서 w로 도착하는 직코스(distance[w][0])가 더 길면
                    distance[w][0] = distance[u][0] + weight[u][w];   //u를 경유해서 w로 도학하는 거리로 업데이트!
                    distance[w][1] = u;   //경유지점 저장
                }
    }
}
//깊이 우선 탐색 - 지하철 역 전체 탐색(v는 번지수)
void dif_mat(GraphType* g, int v) {
    int w;
    g->visited[v] = TRUE;
    printf("%s\n", g->metro[v]);   //해당 번지의 지하철 역 출력
    for (w = 0;w < g->n;w++) {
        if (g->adj_mat[v][w] && !g->visited[w])
            dif_mat(g, w);
    }
}

int main() {
    GraphType * g;
    char s_metro[15], d_metro[15];  //사용자 입력할 출발지, 도착지
    int start = -1, depart = -1; //출발 번지, 도착 번지
    int i;

    g = (GraphType*)malloc(sizeof(GraphType));
    InitGraph(g); //그래프 초기화
    //각 정점에 지하철 역이름 할당
    strcpy(g->metro[0], "노량진");
    strcpy(g->metro[1], "서울역");
    strcpy(g->metro[2], "동작");
    strcpy(g->metro[3], "여의도");
    strcpy(g->metro[4], "신길");
    strcpy(g->metro[5], "동대문역사공원");
    strcpy(g->metro[6], "용산");
    strcpy(g->metro[7], "동대문");
    strcpy(g->metro[8], "삼각지");
    strcpy(g->metro[9], "당산");
    strcpy(g->metro[10], "영등포구청");
    strcpy(g->metro[11], "신도림");
    strcpy(g->metro[12], "종로3가");
    strcpy(g->metro[13], "공덕");
    strcpy(g->metro[14], "합정");
    strcpy(g->metro[15], "오목교");
    strcpy(g->metro[16], "까치산");
    strcpy(g->metro[17], "시청");
    strcpy(g->metro[18], "홍대입구");
    strcpy(g->metro[19], "디지털시티");
    g->n = 20;   //지하철 역 20개

    //엣지 연결하기
    InsertEdge(g, 0, 1);
    InsertEdge(g, 0, 2);
    InsertEdge(g, 0, 3);
    InsertEdge(g, 0, 4);
    InsertEdge(g, 1, 5);
    InsertEdge(g, 1, 6);
    InsertEdge(g, 1, 8);
    InsertEdge(g, 2, 8);
    InsertEdge(g, 3, 9);
    InsertEdge(g, 3, 13);
    InsertEdge(g, 3, 4);
    InsertEdge(g, 4, 10);
    InsertEdge(g, 4, 11);
    InsertEdge(g, 5, 12);
    InsertEdge(g, 5, 7);
    InsertEdge(g, 5, 17);
    InsertEdge(g, 6, 17);
    InsertEdge(g, 7, 12);
    InsertEdge(g, 8, 13);
    InsertEdge(g, 9, 14);
    InsertEdge(g, 9, 10);
    InsertEdge(g, 10, 11);
    InsertEdge(g, 10, 15);
    InsertEdge(g, 11, 16);
    InsertEdge(g, 12, 13);
    InsertEdge(g, 12, 17);
    InsertEdge(g, 13, 14);
    InsertEdge(g, 13, 18);
    InsertEdge(g, 14, 18);
    InsertEdge(g, 14, 19);
    InsertEdge(g, 15, 16);
    InsertEdge(g, 17, 18);
    InsertEdge(g, 18, 19);



    printf("\n출발역: ");
    gets(s_metro);
    printf("도착역: ");
    gets(d_metro);
    for (i = 0;i < MAX_VERTEX;i++)
        if (!strcmp(g->metro[i], s_metro)) {
            start = i;
            break;
        }
    for (i = 0;i < MAX_VERTEX;i++)
        if (!strcmp(g->metro[i], d_metro)) {
            depart = i;
            break;
        }
    if (start == -1 || depart == -1) {
        printf("역 이름 잘못 입력하였습니다.\n");
        exit(-1);
    }

    int path[MAX_VERTEX]; //경로 - 거꾸로추적가능
    for (i = 0;i < MAX_VERTEX;i++)
        path[i] = -1;  //초기화
    shortest_path(start, g->n); //최단경로 구하기

    path[0] = depart;
    path[1] = distance[depart][1];  //start -> depart까지 경유지 체크
    i = 1;
    while (path[i] != start) {
        path[i + 1] = distance[path[i]][1];
        i++;
    } //반복문 나오면 path[i] == start
    while (i > 0)
        printf("%s - ", g->metro[path[i--]]);
    printf("%s (%.2f km)\n\n", g->metro[path[0]],distance[depart][0]);
}