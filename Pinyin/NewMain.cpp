#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <stack>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include "TranslateTree.cpp"

using namespace std;

int main() {
    ofstream fout("logTrans.txt");
    fout << "开始程序" << endl;
    fout.close();

    CStep* pInputStep = new CStep();
    
    string input;
    cout << "input pinyin" << endl;
    char ch;
    string s;
    vector<char*> vecSegRes;
    while (1){
        s = "";
        while (!kbhit()){}
        int line = 0;
        ch = getch();
        if (27 == ch) {
            break;
        } else if (8 == ch) {
            if (input.size() > 0) {
                input = input.substr(0, input.size() - 1);
                system("cls");
                cout << input << endl;
                pInputStep->DeleteStep();
                vecSegRes = pInputStep->GetTransRes();
                for (int i = 0; i < vecSegRes.size(); i++) {
                    cout << UTF8ToGBK(vecSegRes[i]) << endl;
                }
            }
            else {
                input = "";
                cout << SCREATE << " " << SDELETE << endl;
            }
        } else if (ch >= 'a' && ch <= 'z') {
            system("cls");
            s += ch;
            input += ch;
            cout << input << endl;
            pInputStep->InputStep(s);
            vecSegRes = pInputStep->GetTransRes();
            pInputStep->PrintLog();
            for (int i = 0; i < vecSegRes.size(); i++) {
                cout << UTF8ToGBK(vecSegRes[i]);
            }
        } else if (ch >= '0' && ch <= '9') {
            system("cls");
            s += ch;
            input += ch;
            cout << input << endl;
            pInputStep->InputStepNine(s);
            vecSegRes = pInputStep->GetTransRes();
            pInputStep->PrintLog();
            for (int i = 0; i < vecSegRes.size(); i++) {
                if (i >= line) {
                    cout << endl;
                    line = line + 20;
                }
                cout << UTF8ToGBK(vecSegRes[i]) << " ";
            }
        }
    }
        
    return 0;
}