#include<iostream>
using namespace std ;

string a()
{
    int ivalue = 8;
    int arr[50];

    string a_str = "";

    for(int i = 0 ; i < 16; i++)
    {
        arr[i] = ivalue % 2;
        ivalue = ivalue / 2;
    }


    for (int i = 16 - 1; i >= 0; i--)
    {
         a_str += to_string(arr[i]);  
    }

    return a_str;
}

int main()
{
  cout <<a() << endl;
}

