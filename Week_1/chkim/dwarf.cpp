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
    return 0;
}


void sort(int* arr, int arr_size) {
    for (int i = 0; i < arr_size - 1; i++) {
        for (int j = 0; j < arr_size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main()
{
    
    int dwarf[9]; //난쟁이들의 키를 저장하는 배열
    int dwarf_permutation[9]; 
    int real_dwarf[7]; //진짜 난쟁이들의 키를 저장하는 배열

    for(int i = 0; i < 9; i++)
    {
        std::string input;
        std::cin >> input;
        dwarf[i] = std::stoi(input); //입력받은 키를 정수로 변환하여 배열에 저장

        if(i<2)
        {
            dwarf_permutation[i] = 0; //난쟁이들을 나누는 경우의 수를 나타내는 배열 -> 0은 가짜 난쟁이, 1은 진짜 난쟁이
        }
        else
        {
            dwarf_permutation[i] = 1;
        }
    }

    while(1)
    {
        int sum = 0;
        int k = 0;
        for(int i = 0; i < 9; i++)
        {
            if(dwarf_permutation[i] == 1)
            {
                real_dwarf[k] = dwarf[i]; //진짜 난쟁이들의 키를 real_dwarf 배열에 저장
                sum += dwarf[i];
                k++;
            }
        }

        if(sum == 100) //진짜 난쟁이들의 키의 합이 100인 경우
        {
            sort(real_dwarf, 7); //진짜 난쟁이들의 키를 오름차순으로 정렬

            for(int i = 0; i < 7; i++)
            {
                std::cout << real_dwarf[i] << std::endl; //진짜 난쟁이들의 키를 출력
            }
            break;
        }

        next_permutation(dwarf_permutation, 9);

    }

    return 0;

}
