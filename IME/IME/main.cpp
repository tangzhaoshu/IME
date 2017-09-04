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
#include "PCtable.hpp"
//#include "PYsegment.cpp"
#include "PYSegNine.cpp"
#include "translate.hpp"
using namespace std;


string UTF8ToGBK(const string& strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    unsigned short *wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCCH)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

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
	cout << "hello" << endl;
    ofstream fout("log.txt");
    fout << "start process" << endl;
    fout.close();

    CHPYTable *cptable = new CHPYTable();
    //CSegment *segment = new CSegment(cptable->py);
	CStepInput *segment = new CStepInput(cptable->py);

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
        
		while (ch = _getch()){
			s = "";
			if (27 == ch) {
				return 0;
			}
			else if (8 == ch) {
				if (input.size() > 0) {
					input = input.substr(0, input.size() - 1);
					trans->DeleteYJ(segment->GetSegment());
					segment->DeleteStep();
					system("cls");
					cout << input << endl;
					segment->output();
					step.pop_back();
					if (step.size() > 0) {
						for (int i = 0; i < step.back().size(); i++) {
							cout << UTF8ToGBK(step.back()[i].first) << endl;
						}
					}
					segment->Log();
					trans->ShowTreePath();
				}
				else {
					input = "";
					system("cls");
				}
			}
			else if ((ch >= 'a' && ch <= 'z') || (ch > '1' && ch <= '9')) {
				s += ch;
				input += ch;
				system("cls");
				cout << input << endl;

				candidateCH.clear();
				output.clear();

				SYSTEMTIME sys;
				GetLocalTime(&sys);
				printf("%4d/%02d/%02d %02d:%02d:%02d.%03d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
				cout << endl;

				segment->AddStep(s);

				segResult = segment->GetSegment();



				for (int i = 0; i < segResult.size(); i++) {
					temp = segment->getpy(segResult[i].back());
					temp = cptable->getChinese(temp);
					candidateCH.push_back(temp);
				}



				translateResult = trans->getTranslate(segResult, candidateCH);

				GetLocalTime(&sys);
				printf("%4d/%02d/%02d %02d:%02d:%02d.%03d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
				cout << endl;
				cout << endl;

				trans->ShowTreePath();
				//     segment->output();
				segment->Log();

				int flag = 0;
				for (int i = 0; i < translateResult.size(); i++) {
					translateResult[i]->print();
					for (int j = 0; j < translateResult[i]->trans_Total.size(); j++) {
						if (translateResult[i]->trans_Total[j]->minSeg == 0) {
							continue;
						}
						if (translateResult[i]->trans_Total[j]->minSeg == 1) {
							output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res,
								translateResult[i]->trans_Total[j]->prob));
							flag = 2;
						}
						else {
							if (flag == 0) {
								output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res,
									translateResult[i]->trans_Total[j]->prob));
								flag = 1;
							}
							else if (flag == 1 && j == 0) {
								if (translateResult[i]->trans_Total[j]->prob < output.back().second) {
									output.pop_back();
									output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res,
										translateResult[i]->trans_Total[j]->prob));
								}
							}
						}
					}
				}
				sort(output.begin(), output.end(), cmpYJNode);
				for (int i = 0; i < output.size(); i++) {
					cout << UTF8ToGBK(output[i].first) << endl;
				}
				cout << endl;
				step.push_back(output);
			}

		}

    }
    return 0;
}