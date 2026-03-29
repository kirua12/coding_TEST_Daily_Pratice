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

int calculate(int x, int y,int op)
{
    int result =0;
    if(op == 0)
    {
        result = x+y;
    }
    else if(op == 1)
    {
        result = x-y;
    }
    else if(op == 2)
    {
        result = x*y;
    }
    else if(op == 3)
    {
        result = x/y;
    }



    return result;
}

int main()
{
    int num_number =0;
    std::cin >> num_number;// 넘버 갯수 받고

    int* numbers = new int [num_number]; //넘버 크기만큼 할당 받고

    int* operator_permutation = new int [num_number-1]; //넘버 크기만큼 할당 받고


    for(int i=0; i < num_number ; i++)
    {
        std::cin >> numbers[i]; //어떤 숫자들이 있는지 받기
    }

    int count =0;
    for(int i =0; i<4;i++)
    {
        int operator_count;
        std::cin >> operator_count;
        for(int j =0;j<operator_count;j++)
        {
            operator_permutation[count] =i;
            count++;
        }
    }

    int Min_value = __INT_MAX__;
    int Max_value = -Min_value + 1;

    while(1)
    {
        int result =0;

        for(int i=0; i<num_number-1;i++)
        {
            if(i ==0)
                result = calculate(numbers[i],numbers[i+1],operator_permutation[i]);
            else
                result = calculate(result,numbers[i+1],operator_permutation[i]);
        }

        if(Min_value> result)
            Min_value = result;
        
        if(Max_value<result)
            Max_value = result;



        if(count ==-2)
            break;        
        if (next_permutation(operator_permutation, num_number-1) == 5) // 팀을 나누는 경우의 수가 5개 이상인 경우 -> 6명 이상인 경우
        {
            count = -2;
        }
    }
     
    std::cout<< Max_value<< std::endl;
    std::cout<< Min_value<< std::endl;



}
