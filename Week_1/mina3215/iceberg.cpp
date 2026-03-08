#include <iostream>
#include <string>
#include <sstream>


int main()
{
    //std::cout << "input_Team_Size: " ;

    int  col, row= 0;

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


    while(1)
    {
        int **temp_iceberg = new int*[row]; //빙산이 녹기 전 상태를 기억할 Temp_iceBerge
        for(int i = 0; i < row; i++)
        {
            temp_iceberg[i] = new int[col];
        }

        memcpy(temp_iceberg, iceberg, sizeof(int)*row*col); //빙산이 녹기 전 상태를 temp_iceberg에 복사

        for(int i = 0; i<row; i++)
        {
            for(int j = 0; j<col;j++)
            {
                if(temp_iceberg[i][j] !=0)
                {
                    int melt = 0; //녹는 양을 저장하는 변수

                    if(i-1 >= 0 && temp_iceberg[i-1][j] == 0) //상
                    {
                        melt++;
                    }
                    if(i+1 < row && temp_iceberg[i+1][j] == 0) //하
                    {
                        melt++;
                    }
                    if(j-1 >= 0 && temp_iceberg[i][j-1] == 0) //좌
                    {
                        melt++;
                    }
                    if(j+1 < col && temp_iceberg[i][j+1] == 0) //우
                    {
                        melt++;
                    }

                    iceberg[i][j] -= melt; //녹는 양만큼 빙산의 높이 감소

                    if(iceberg[i][j] < 0) //빙산의 높이가 음수가 되는 경우 0으로 설정
                    {
                        iceberg[i][j] = 0;
                    }
                }
            }
        }


        //DFS 구현 공부좀 해야할듯 




        for(int i= 0 ; i<row; i++)
        {
            delete[] temp_iceberg[i];
        }
        delete[] temp_iceberg;
    }












    for(int i= 0 ; i<row; i++)
    {
        delete[] iceberg[i];
    }
    delete[] iceberg;

}