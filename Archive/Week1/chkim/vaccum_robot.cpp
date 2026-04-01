#include <iostream>
#include <string>
#include <sstream>
#include <vector>

bool isValid(int y, int x, int row, int col) {
    return (y >= 0 && y < row && x >= 0 && x < col);
}

void back(int* x_idx,int* y_idx,int direct)
{
    if(direct<0 || direct>3) //예외처리
        return;

    if(direct == 0) //방향이 북쪽일 때
    {
        *y_idx +=1;
    }
    else if(direct ==1)//방향이 동쪽일 때
    {
        *x_idx-=1;
    }
    else if(direct ==2)//방향이 남쪽일 때
    {
        *y_idx-=1;
    }
    else if(direct ==3)//방향이 서쪽일 때
    {
        *x_idx+=1;
    }
}

void front(int* x_idx,int* y_idx,int direct)
{
    if(direct<0 || direct>3) //예외처리
        return;

    if(direct == 0) //방향이 북쪽일 때
    {
        *y_idx -=1;
    }
    else if(direct ==1)//방향이 동쪽일 때
    {
        *x_idx+=1;
    }
    else if(direct ==2)//방향이 남쪽일 때
    {
        *y_idx+=1;
    }
    else if(direct ==3)//방향이 서쪽일 때
    {
        *x_idx-=1;
    }
}


void turn(int *direct)
{
    if(*direct<0 || *direct > 3) //예외처리
        return;

    if(*direct == 0) //방향이 북쪽일 때
    {
        *direct = 3;
    }
    else if(*direct ==1)//방향이 동쪽일 때
    {
        *direct = 0;
    }
    else if(*direct ==2)//방향이 남쪽일 때
    {
        *direct = 1;
    }
    else if(*direct ==3)//방향이 서쪽일 때
    {
        *direct = 2;
    }
}


int main()
{
    
    int col, row = 0; //방의 크기

    std::cin >> row;
    std::cin >> col;

    int x,y,direct = 0; // 로봇의 초기 상태

    std::cin >> y;
    std::cin >> x;
    std::cin >> direct;


    int **room = new int*[row];
    for (int i = 0; i < row; i++) {
        room[i] = new int[col];
    }

    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            std::cin >> room[i][j];
        }
    }

    int count = 0;
    int back_count = 0;
    while(1)
    {
        if(room[y][x] == 0)
        {
            room[y][x] = 2;
            count++;
            back_count =0;
        }

        if(room[y][x] == 2)
        {
            if((isValid(y-1,x,row,col)&& (room[y-1][x] == 2 || room[y-1][x] == 1)) &&
                (isValid(y+1,x,row,col)&& (room[y+1][x] == 2 || room[y+1][x] == 1)) &&
                (isValid(y,x-1,row,col)&& (room[y][x-1] == 2 ||room[y][x-1] == 1)) &&
                (isValid(y,x+1,row,col)&& (room[y][x+1] == 2 || room[y][x+1] == 1)))
            {
                back(&x,&y,direct);
            }
            else if((isValid(y-1,x,row,col)&& room[y-1][x] == 0) ||
                (isValid(y+1,x,row,col)&& room[y+1][x] == 0) ||
                (isValid(y,x-1,row,col)&& room[y][x-1] == 0) ||
                (isValid(y,x+1,row,col)&& room[y][x+1] == 0))
            {
                back_count =0;
                while(1)
                {
                    turn(&direct);
                    front(&x, &y, direct);
                    if (!isValid(y, x, row, col) || room[y][x] == 1 || room[y][x] == 2)
                    {
                        back(&x, &y, direct);
                    }
                    else{
                        break;
                    }
                }
                
                
     

            }
        }
        else if(room[y][x] == 1)
        {
            if(back_count ==0)
            {
                front(&x,&y,direct);
                back_count++;
            }
            else
                break;
        }


        
    }

    std::cout << count;

    for(int i=0; i<row; i++)
    {
        delete[] room[i];
    }
    delete[] room;

    
}
