#include <iostream>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <string>
using namespace std;
int GetDirection()
{
    int ret = 0;
 
    do 
    {
        int ch = getch();
        if(isascii(ch))
            continue;
 
        ch = getch();
        switch(ch)
        {
        case 72:   
            ret = 2; // top
            break;
        case 75:   
            ret = 1; // left 
            break;
        case 77:   
            ret = 3; // right
            break;
        case 80:   
            ret = 4; // down
            break;
        default:   
            break;
        }
    } while (ret == 0);
     
    return ret;
}
 
int main()
{
    char ch;
    string str = "";
    while (1){
        while (!kbhit()){
        }
        
        ch = getch();
        if (27 == ch) {
            break;
        }
        if (8 == ch) {
            if (str.size() > 0) {
                str = str.substr(0, str.size() - 1);
            }
        } else {
            str += ch;
        }
        system("cls");
        cout << str;
    }
    
    system("pause");
    return 0;
}