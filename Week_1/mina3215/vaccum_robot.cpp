#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int main()
{
    int N = 0;

    std::cin >> N;

    int **room = new int*[N]; // 방의 상태를 입력 받은 만큼 할당함 2차원 배열로

    for (int i = 0; i < N; i++) {
        room[i] = new int[N];
    }

    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            std::string input;
            std::cin >> input;
            room[i][j] = std::stoi(input); //입력받은 방의 상태를 정수로 변환하여 배열에 저장 // 내잔디 어디갔어
        }
    }


}
