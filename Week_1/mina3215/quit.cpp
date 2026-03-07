#include <iostream>
#include <string>
#include <sstream>
#include <vector>

int next_permutation(int* arr, int arr_size) {
    int i = arr_size - 1;
    while (i > 0 && arr[i - 1] >= arr[i]) {
        i--;
    }
    if (i <= 0) {
        return 5; // 마지막 순열인 경우
    }

    int j = arr_size - 1;
    while (arr[j] <= arr[i - 1]) {
        j--;
    }

    std::swap(arr[i - 1], arr[j]);

    j = arr_size - 1;
    while (i < j) {
        std::swap(arr[i], arr[j]);
        i++;
        j--;
    }
}

int main()
{
    std::cout << "input_Team_Size: " ;

    int N = 0;

    std::cin >> N;

    if(N % 2 != 0) //문제에서는 짝수만 들어온다고 되어있지만 예외처리는 해주는 습관을 들여놓자
    {
        std::cout << "not even number try again" << std::endl;

        return 0;
    }

    int **team_senerge = new int*[N ]; // 팀 시너지 배열을 입력 받은 만큼 할당함 2차원 배열로
    int *human = new int[N];

    for (int i = N/2; i < N; i++) {
        team_senerge[i] = new int[N];
        human[i] = 1; // 1로 초기화 -> 나중에 팀을 나눌 때 사용하기 위해서
    }

    for(int i = 0; i < N; i++)
    {
       
        for(int j = 0; j < N; j++)
        {
            std::string input;
            std::cin >> input;
            team_senerge[i][j] = std::stoi(input); //입력받은 시너지를 정수로 변환하여 배열에 저장
        }
    }


    //여기서 부터는 입력만 받는거 이제 알고리즘이 들어가야함 -> 결국 확률임
    //4 명일때는 한명이 하나만 선택하면 나머지 한명은 자동으로 선택됨
    //하지만 6명 이상의 경우 확률이 증가함 그것에 대한 수식을 확립해야함 -> 매우 중요!
    //결국 full search로 모든 경우의 수를 탐색
    int MIN_SENERGE_DIFF = 100000000; //시너지 차이의 최솟값을 저장할 변수 -> 초기값은 매우 큰 수로 설정
    while(1)
    {
        if(next_permutation(human, N) == 5) //팀을 나누는 경우의 수가 5개 이상인 경우 -> 6명 이상인 경우
        {
        break;
        }
        if(human[0] ==0)
        {
            int team1_senerge = 0;
            int team2_senerge = 0;

            for(int i = 0; i < N; i++)
            {
                for(int j = 0; j < N; j++)
                {
                    if(human[i] == 0 && human[j] == 0) //팀1에 속한 사람들끼리의 시너지 계산
                    {
                        team1_senerge += team_senerge[i][j];
                    }
                    else if(human[i] == 1 && human[j] == 1) //팀2에 속한 사람들끼리의 시너지 계산
                    {
                        team2_senerge += team_senerge[i][j];
                    }
                }
            }

            int senerge_diff = std::abs(team1_senerge - team2_senerge); //시너지 차이 계산

            if(senerge_diff < MIN_SENERGE_DIFF) //최솟값 갱신
            {
                MIN_SENERGE_DIFF = senerge_diff;
            }
        }


    }
    



    std::cout << MIN_SENERGE_DIFF << std::endl; //최솟값 출력


    for (int i = 0; i <= N; i++) { //동적할당한것을 지워줘야함 -> 메모리 누수 방지
        delete[] team_senerge[i];
    }
    delete[] team_senerge;

    return 0;


}