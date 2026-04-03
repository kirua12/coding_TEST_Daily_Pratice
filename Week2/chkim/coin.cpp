#include <iostream>


int main()
{
    int number_count =0;

    std::cin>> number_count;
    int* coin = new int[number_count];

    int total_coin =0;

    std::cin >> total_coin;

    for(int i=0; i< number_count; i++)
    {
        std::cin>> coin[i];
    }

    int result_count =0;
    //while(1) 생각해보니 필요 없을것같아 지움
    {
        
        for (int i = number_count-1; i >=0; i--)
        {
            if(total_coin>=coin[i])
            {
                int quotient = total_coin/coin[i];
                total_coin = total_coin - coin[i]*quotient;
                result_count+= quotient;
                
            }

        }



    }

    std::cout<< result_count;

    delete[] coin;
}