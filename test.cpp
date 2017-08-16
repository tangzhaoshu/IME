#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
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

string UTF8ToGBK(const string& strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    unsigned short *wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP,0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
    //strUTF8 = szGBK;
    std::string strTemp(szGBK);
    delete[]szGBK;
    delete[]wszGBK;
    return strTemp;
}

 
int main()
{
    fstream fin("Pinyin/result.txt");
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    string str;
    getline(fin, str);
    cout << UTF8ToGBK(str) << endl;
    fin.close();
    char ch;
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