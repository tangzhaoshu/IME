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
#include "PYsegment.cpp"
#include "translate.cpp"
using namespace std;






int cmpYJNode(const pair<string, double> &x, const pair<string, double> &y) {
	return x.second < y.second;
}


int main() {
	cout << "hello world" << endl;
	ofstream fout("log.txt");
	fout << "¿ªÊ¼³ÌÐò" << endl;
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
		while (ch = _getch()){
			// ch = getch();
			if (27 == ch) {
				break;
			}
			else if (8 == ch) {
				if (input.size() > 0) {
					input = input.substr(0, input.size() - 1);
					trans->DeleteYJ(segment->GetSegment());
					segment->DeleteChar();
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
			else if (ch >= 'a' && ch <= 'z') {
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

				segment->InputChar(s);

				segResult = segment->GetSegment();



				for (int i = 0; i < segResult.size(); i++) {
					temp = segment->pytree->getpy(segResult[i].back());
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