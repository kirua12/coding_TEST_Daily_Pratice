#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

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

    int year_count =0; //빙산이 녹는 년도를 저장하는 변수
    while(1)
    {
        int **temp_iceberg = new int*[row]; //빙산이 녹기 전 상태를 기억할 Temp_iceBerge
        for(int i = 0; i < row; i++)
        {
            temp_iceberg[i] = new int[col];
        }

        int **label = new int*[row]; //빙산이 녹기 전 상태를 기억할 Temp_iceBerge
        for(int i = 0; i < row; i++)
        {
            label[i] = new int[col];
        }

        for (int i = 0; i < row; i++)
        {
            memcpy(temp_iceberg[i], iceberg[i], sizeof(int) * col);
        }


        for(int i = 0; i<row; i++)
        {
            for(int j = 0; j<col;j++)
            {
                label[i][j] = 0; //label 초기화
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


        //labeling 알고리즘 구현
        int label_count = 1;

        for(int i= 0 ; i<row; i++)
        {
            for(int j= 0 ; j<col; j++)
            {
                if(iceberg[i][j] != 0)
                {
                    if(i-1 >= 0 && label[i-1][j] != 0) //상
                    {
                        label[i][j] = label[i-1][j];
                    }
 
                    else if(j-1 >= 0 && label[i][j-1] != 0) //좌
                    {
                        label[i][j] = label[i][j-1];
                    }
                    else if(j+1 < col && label[i][j+1] != 0) //우
                    {
                        label[i][j] = label[i][j+1];
                    }
                    else if(i+1 < row && label[i+1][j] != 0) //하
                    {
                        label[i][j] = label[i+1][j];
                    }
                    else
                    {
                        label[i][j] = label_count; //새로운 라벨 할당
                        label_count++;
                    }
                    
                }
            }
        }

        if(label_count ==1)
        {
            for (int i = 0; i < row; i++)
            {
                delete[] label[i];
            }
            delete[] label;
            break; //빙산이 모두 녹은 경우
        }

        int* max_label = new int[label_count-1]; //각 라벨의 개수를 저장하는 배열
        memset(max_label, 0, sizeof(int) * (label_count-1)); //max_label 초기화

        for(int i= 0 ; i<row; i++)
        {
            for(int j= 0 ; j<col; j++)
            {
                if(j+1<col && label[i][j] != 0 && label[i][j] != label[i][j+1] && label[i][j] > label[i][j+1]&& label[i][j+1] != 0) //우
                {
                    max_label[label[i][j]-1] = label[i][j+1];
                }
                else if(i-1>=0 && label[i][j] != 0 && label[i][j] != label[i-1][j] && label[i][j] > label[i-1][j] && label[i-1][j] != 0) //상
                {
                    max_label[label[i][j]-1] = label[i-1][j];
                }
                else if(i+1<row && label[i][j] != 0 && label[i][j] != label[i+1][j] && label[i][j] > label[i+1][j] && label[i+1][j] != 0) //하
                {
                    max_label[label[i][j]-1] = label[i+1][j];
                }
                else if(j-1>=0 && label[i][j] != 0 && label[i][j] != label[i][j-1] && label[i][j] > label[i][j-1] && label[i][j-1] != 0) //좌
                {
                    max_label[label[i][j]-1] = label[i][j-1];
                }
                 else if(label[i][j] != 0 && max_label[label[i][j]] == 0)
                {
                    max_label[label[i][j]-1] = label[i][j];
                }
            }
        }

        // 아래 주석은 디버깅용
        // std::cout <<std::endl;
        // for(int i= 0 ; i<row; i++)
        // {
        //     for(int j= 0 ; j<col; j++)
        //     {
        //         std::cout << iceberg[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }



        // std::cout <<std::endl;
        // for(int i= 0 ; i<row; i++)
        // {
        //     for(int j= 0 ; j<col; j++)
        //     {
        //         std::cout << label[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }

        //라벨링이 안되는 문제가 있는데 뭐가 문제일까에 대한 분석을 해야함

        int iceberg_count = 0; //빙산의 개수를 저장하는 변수
        for(int i=1;i<label_count-1;i++)
        {
            if(max_label[i] != 1)
            {
                iceberg_count++;
            }
        }
        
        year_count++;
        if(iceberg_count >= 1) //빙산이 2개 이상인 경우
        {
            for (int i = 0; i < row; i++)
            {
                delete[] label[i];
            }
            delete[] label;
            delete[] max_label;
            break;
        }


        for(int i= 0 ; i<row; i++)
        {
            delete[] label[i];
        }
        delete[] label;
        delete[] max_label;
    }




    std::cout << year_count << std::endl;


    for(int i= 0 ; i<row; i++)
    {
        delete[] iceberg[i];
    }
    delete[] iceberg;

    system("pause");
    return 0;

}