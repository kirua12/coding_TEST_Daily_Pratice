#include <iostream>
#include <string>


int main()
{
    std::string alpha = "abcdefghijklmnopqrstuvwxyz";
    int checker_alpha[26] = {0};

    std::string input;

    std::cin >> input;

    for(int i=0; i<input.size();i++)
    {   
        for(int j= 0; j<26;j++)
        {
            if(input.c_str()[i]==alpha.c_str()[j])
            {
                checker_alpha[j]++;
                break;
            }
        }
    }

    for(int i=0; i<26; i++)
    {
        std::cout<< checker_alpha[i]<<" ";
    }




}