#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include "PCtable.cpp"
#include "PYsegment.cpp"
#include "translate.cpp"
using namespace std;


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



int cmpYJNode(const pair<string, double> &x, const pair<string, double> &y) {
    return x.second < y.second;
}


int main() {
    ofstream fout("log.txt");
    fout << "开始程序" << endl;
    fout.close();

    CHPYTable *cptable = new CHPYTable();
    CSegment *segment = new CSegment(cptable->py);
    TranslateTree *trans = new TranslateTree();

    vector<vector<string>> segResult;
    vector<vector<string>> candidateCH;
    vector<string> temp;
    vector<YJNode*> translateResult;
    vector<pair<string, double>> output;
    vector<vector<pair<string, double>>> step;

    string input;
    cout << "input pinyin" << endl;
    char ch;
    string s;
    while (1){
        s = "";
        while (!kbhit()){}
        ch = getch();
        if (27 == ch) {
            break;
        } else if (8 == ch) {
            if (input.size() > 0) {
                input = input.substr(0, input.size() - 1);
                trans->DeleteYJ(segment->GetSegment());
                segment->DeleteChar();
                system("cls");
                cout << input << endl;
                segment->output();
                step.pop_back();
                if (step.size() > 0) {
                    for(int i = 0; i < step.back().size(); i ++) {
                        cout << UTF8ToGBK(step.back()[i].first) << endl;
                    }
                }
                segment->Log();
                trans->ShowTreePath();
            } else {
                input = "";
                system("cls");
            }
        } else if (ch >= 'a' && ch <= 'z') {
            s += ch;
            input += ch;
            system("cls");
            cout << input << endl;
            
            candidateCH.clear();
            output.clear();
    
            SYSTEMTIME sys;
            

            segment->InputChar(s);
            segment->output();
            segResult = segment->GetSegment();
            segment->Log();


            for (int i = 0; i < segResult.size(); i ++) {
                temp = segment->pytree->getpy(segResult[i].back());
                temp = cptable->getChinese(temp);
                candidateCH.push_back(temp);
            }

            GetLocalTime(&sys);
            printf("%4d/%02d/%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
            cout << endl;

            translateResult = trans->getTranslate(segResult, candidateCH);

            GetLocalTime(&sys);
            printf("%4d/%02d/%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
            cout << endl;

            trans->ShowTreePath();

            
            
            int flag;
            for (int i = 0; i < translateResult.size(); i ++) {
                translateResult[i]->print();
                flag = 0;
                for (int j = 0; j < translateResult[i]->trans_Total.size(); j ++) {
                    if (translateResult[i]->trans_Total[j]->minSeg == 1) {
                        output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res, 
                            translateResult[i]->trans_Total[j]->prob));
                        flag = 1;
                    } else {
                        if (flag == 0) {
                            output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res, 
                            translateResult[i]->trans_Total[j]->prob));
                            flag = 1;
                        }
                    }
                }
            }
            sort(output.begin(), output.end(), cmpYJNode);
            for(int i = 0; i < output.size(); i ++) {
                cout << UTF8ToGBK(output[i].first) << endl;
            }
            cout << endl;
            step.push_back(output);
        }
        
        

    }
    return 0;
}