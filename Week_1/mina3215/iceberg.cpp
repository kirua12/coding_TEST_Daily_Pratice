#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <queue>

int main()
{
    int col, row = 0;

    std::cin >> row;
    std::cin >> col;

    int **iceberg = new int*[row];
    for (int i = 0; i < row; i++) {
        iceberg[i] = new int[col];
    }

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            std::cin >> iceberg[i][j];
        }
    }

    int year_count = 0; //빙산이 녹는 년도를 저장하는 변수
    while(1)
    {
        int **temp_iceberg = new int*[row]; //빙산이 녹기 전 상태를 기억할 Temp_iceBerge
        for(int i = 0; i < row; i++)
        {
            temp_iceberg[i] = new int[col];
        }

        bool **visited = new bool*[row]; //BFS용 방문 배열
        for(int i = 0; i < row; i++)
        {
            visited[i] = new bool[col];
            memset(visited[i], false, sizeof(bool) * col);
        }

        for (int i = 0; i < row; i++)
        {
            memcpy(temp_iceberg[i], iceberg[i], sizeof(int) * col);
        }

        // 빙산 녹이기 (temp_iceberg 기준으로 계산)
        for(int i = 0; i < row; i++)
        {
            for(int j = 0; j < col; j++)
            {
                if(temp_iceberg[i][j] != 0)
                {
                    int melt = 0; //녹는 양을 저장하는 변수

                    if(i-1 >= 0 && temp_iceberg[i-1][j] == 0) melt++; //상
                    if(i+1 < row && temp_iceberg[i+1][j] == 0) melt++; //하
                    if(j-1 >= 0 && temp_iceberg[i][j-1] == 0) melt++; //좌
                    if(j+1 < col && temp_iceberg[i][j+1] == 0) melt++; //우

                    iceberg[i][j] -= melt;
                    if(iceberg[i][j] < 0) iceberg[i][j] = 0;
                }
            }
        }

        // BFS로 연결된 빙산 덩어리 개수 세기
        int label_count = 0;

        int *qx = new int[row * col];
        int *qy = new int[row * col];

        for(int i = 0; i < row; i++)
        {
            for(int j = 0; j < col; j++)
            {
                if(iceberg[i][j] != 0 && !visited[i][j])
                {
                    label_count++;
                    int head = 0, tail = 0;
                    qx[tail] = i; qy[tail] = j; tail++;
                    visited[i][j] = true;

                    while(head < tail)
                    {
                        int x = qx[head];
                        int y = qy[head];
                        head++;

                        if(x-1 >= 0 && iceberg[x-1][y] != 0 && !visited[x-1][y])
                        { visited[x-1][y] = true; qx[tail] = x-1; qy[tail] = y; tail++; }
                        if(x+1 < row && iceberg[x+1][y] != 0 && !visited[x+1][y])
                        { visited[x+1][y] = true; qx[tail] = x+1; qy[tail] = y; tail++; }
                        if(y-1 >= 0 && iceberg[x][y-1] != 0 && !visited[x][y-1])
                        { visited[x][y-1] = true; qx[tail] = x; qy[tail] = y-1; tail++; }
                        if(y+1 < col && iceberg[x][y+1] != 0 && !visited[x][y+1])
                        { visited[x][y+1] = true; qx[tail] = x; qy[tail] = y+1; tail++; }
                    }
                }
            }
        }

        delete[] qx; delete[] qy;
        for(int i = 0; i < row; i++) delete[] visited[i];
        delete[] visited;
        for(int i = 0; i < row; i++) delete[] temp_iceberg[i];
        delete[] temp_iceberg;

        if(label_count == 0) { year_count = 0; break; }  //다 녹은 경우
        year_count++;
        if(label_count >= 2) break;  //분리된 경우
    }

    std::cout << year_count << std::endl;

    for(int i = 0; i < row; i++) delete[] iceberg[i];
    delete[] iceberg;

    return 0;
}
