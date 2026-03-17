#include <iostream>
#include <cstring>
#include <queue>

int row, col;
int iceberg[300][300];
int temp_iceberg[300][300];
bool visited[300][300];

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

// BFS로 연결된 빙산 덩어리 개수 세기
int countComponents() {
    memset(visited, false, sizeof(visited));
    int count = 0;

    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (iceberg[i][j] != 0 && !visited[i][j]) {
                count++;
                std::queue<std::pair<int,int>> q;
                q.push({i, j});
                visited[i][j] = true;
                while (!q.empty()) {
                    auto [x, y] = q.front(); q.pop();
                    for (int d = 0; d < 4; d++) {
                        int nx = x + dx[d];
                        int ny = y + dy[d];
                        if (nx >= 0 && nx < row && ny >= 0 && ny < col
                            && !visited[nx][ny] && iceberg[nx][ny] != 0) {
                            visited[nx][ny] = true;
                            q.push({nx, ny});
                        }
                    }
                }
            }
        }
    }
    return count;
}

int main() {
    std::cin >> row >> col;

    for (int i = 0; i < row; i++)
        for (int j = 0; j < col; j++)
            std::cin >> iceberg[i][j];

    int year_count = 0;

    while (true) {
        int components = countComponents();

        if (components == 0) {
            // 빙산이 다 녹았는데 분리 안 됨 -> 0 출력
            std::cout << 0 << std::endl;
            return 0;
        }
        if (components >= 2) {
            // 빙산이 분리됨
            std::cout << year_count << std::endl;
            return 0;
        }

        // 빙산 녹이기: temp에 현재 상태 백업 후 처리
        memcpy(temp_iceberg, iceberg, sizeof(iceberg));

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (temp_iceberg[i][j] != 0) {
                    int melt = 0;
                    for (int d = 0; d < 4; d++) {
                        int nx = i + dx[d];
                        int ny = j + dy[d];
                        if (nx >= 0 && nx < row && ny >= 0 && ny < col
                            && temp_iceberg[nx][ny] == 0) {
                            melt++;
                        }
                    }
                    iceberg[i][j] = std::max(0, iceberg[i][j] - melt);
                }
            }
        }

        year_count++;
    }

    return 0;
}
