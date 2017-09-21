#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include "TransTree.hpp"
//#include "nineInput.hpp"

using namespace std;


void TestNew() {
	fstream fin("inputFull.txt");
	if (!fin) {
		cout << "open file error" << endl;
		exit(1);
	}
	string str;
	vector<string> testData;
	vector<char*> decodeRes;
	int decodeNum = 0;
	while (getline(fin, str)) {
		testData.push_back(str);
		cout << str << "  " << str.size() << endl;
	}
	fin.close();

	CStep* pInputStep = new CStep();

	string input;
	cout << "input pinyin" << endl;
	char ch;
	string s;
	vector<char*> vecSegRes;

	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值

	for (int i = 0; i < testData.size(); i++) {
		cout << testData[i] << endl;
		for (int j = 0; j < testData[i].size(); j++) {
			s = "";
			decodeNum++;
			s += testData[i][j];
			pInputStep->InputStepNew(s);
		}
		vecSegRes = pInputStep->GetTransRes();
		if (vecSegRes.size() > 0) {
			int len = strlen(vecSegRes[0]) + 1;
			char* pResult = new char[len];
			for (int j = 0; j < len - 1; j++) {
				pResult[j] = vecSegRes[0][j];
			}
			pResult[len - 1] = '\0';
			decodeRes.push_back(pResult);
		}
		vecSegRes.clear();
		while (!pInputStep->IsEmpty()) {
			//     decodeNum ++;
			pInputStep->DeleteStep();
		}

		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2 - QPart1);
		dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒 
		cout << dfTim / decodeNum << endl;
		//    DWORD dwUsed = GetTickCount() - dwStart;
		//    cout << dwUsed / decodeNum << endl;
	}

	ofstream fout("log.txt");
	for (int i = 0; i < decodeRes.size(); i++) {
		fout << decodeRes[i] << endl;
		delete[] decodeRes[i];
	}
	fout.close();
	delete pInputStep;
}


void TestNine() {
	fstream fin("inputNine.txt");
	if (!fin) {
		cout << "open file error" << endl;
		exit(1);
	}
	string str;
	vector<string> testData;
	vector<char*> decodeRes;
	int decodeNum = 0;
	while (getline(fin, str)) {
		testData.push_back(str);
		cout << str << "  " << str.size() << endl;
	}
	fin.close();
	CStep* pInputStep = new CStep();
	string input;
	cout << "input pinyin" << endl;
	char ch;
	string s;
	vector<char*> vecSegRes;
	DWORD dwStart = GetTickCount();
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值

	for (int i = 0; i < testData.size(); i++) {
		cout << testData[i] << endl;
		for (int j = 0; j < testData[i].size(); j++) {
			s = "";
			decodeNum++;
			s += testData[i][j];
			pInputStep->InputStepNine(s);
		}
		vecSegRes = pInputStep->GetTransRes();
		if (vecSegRes.size() > 0) {
			int len = strlen(vecSegRes[0]) + 1;
			char* pResult = new char[len];
			for (int j = 0; j < len - 1; j++) {
				pResult[j] = vecSegRes[0][j];
			}
			pResult[len - 1] = '\0';
			decodeRes.push_back(pResult);
		}
		vecSegRes.clear();
		while (!pInputStep->IsEmpty()) {
			pInputStep->DeleteStep();
		}
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2 - QPart1);
		dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒 
		cout << dfTim / decodeNum << endl;
	}
	DWORD dwUsed = GetTickCount() - dwStart;
	cout << dwUsed / decodeNum << endl;

	ofstream fout("log.txt");
	for (int i = 0; i < decodeRes.size(); i++) {
		fout << decodeRes[i] << endl;
		delete[] decodeRes[i];
	}
	fout.close();
	delete pInputStep;
}


int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	TestNew();
//	TestNine();
	//ofstream fout("logNew.txt");
	//fout << "开始程序" << endl;
	//fout.close();

	//CStep* pInputStep = new CStep();

	//string input;
	//cout << "input pinyin" << endl;
	//char ch;
	//string s;
	//vector<char*> vecSegRes;
	//while (ch = _getch()){
	//	s = "";
	//	if (27 == ch) {
	//		break;
	//	}
	//	else if (8 == ch) {
	//		cout << 1;
	//		if (input.size() > 0) {
	//			input = input.substr(0, input.size() - 1);
	//			//system("cls");
	//			cout << input << endl;
	//			pInputStep->DeleteStep();
	//			vecSegRes = pInputStep->GetTransRes();
	//			for (int i = 0; i < vecSegRes.size(); i++) {
	//				cout << UTF8ToGBK(vecSegRes[i]) << endl;
	//			}
	//		}
	//		else {
	//			input = "";
	//			system("cls");
	//		}
	//	}
	//	else if (ch >= 'a' && ch <= 'z') {
	//		s += ch;
	//		input += ch;
	//		//system("cls");
	//		cout << input << endl;
	//		pInputStep->InputStepNew(s);
	//		vecSegRes = pInputStep->GetTransRes();
	//		for (int i = 0; i < vecSegRes.size(); i++) {
	//			cout << UTF8ToGBK(vecSegRes[i]) << endl;
	//		}
	//	}
	//
	//	
	//	
	//}
		//ofstream fout("log.txt");
		//fout << "开始程序" << endl;
		//fout.close();
	return 0;
}







//#include <iostream>
//#include <vector>
//#include <fstream>
//#include <map>
//#include <set>
//#include <string>
//#include <conio.h>
//#include <windows.h>
//#include <process.h>
//#include <algorithm>
//#include <cstdlib>
//#include <time.h>
////#include "PCtable.hpp"
////#include "PYsegment.cpp"
////#include "PYSegNine.cpp"
////#include "translate.hpp"
//#include "TransTree.hpp"
//using namespace std;
//
//
//
//
//
//int cmpYJNode(const pair<string, double> &x, const pair<string, double> &y) {
//    return x.second < y.second;
//}
//
//
//
//int main() {
//	ofstream fout("log.txt");
//	fout << "开始程序" << endl;
//	fout.close();
//
//	CStep* pInputStep = new CStep();
//
//	string input;
//	cout << "input pinyin" << endl;
//	char ch;
//	string s;
//	vector<char*> vecSegRes;
//	while (cin >> s) {
//		input += s;
//		system("cls");
//		cout << input << endl;
//		pInputStep->InputStep(s);
//		vecSegRes = pInputStep->GetTransRes();
//		for (int i = 0; i < vecSegRes.size(); i++) {
//			cout << vecSegRes[i] << endl;
//		}
//	}
//
//	//ofstream fout("log.txt");
//	//fout << "开始程序" << endl;
//	//fout.close();
//
//	////CHPYTable *cptable = new CHPYTable();
//
//	CStep* pInputStep = new CStep();
//
//	string input;
//	cout << "input pinyin" << endl;
//	char ch;
//	string s;
//	vector<char*> vecSegRes;
//	while (1){
//		while (cin >> ch){
//			s = "";
//			if (27 == ch) {
//				break;
//			}
//			else if (8 == ch) {
//				cout << 1;
//				if (input.size() > 0) {
//					input = input.substr(0, input.size() - 1);
//					//system("cls");
//					cout << input << endl;
//					pInputStep->DeleteStep();
//					vecSegRes = pInputStep->GetTransRes();
//					for (int i = 0; i < vecSegRes.size(); i++) {
//						cout << vecSegRes[i] << endl;
//					}
//				}
//				else {
//					input = "";
//					system("cls");
//				}
//			}
//			else if (ch >= 'a' && ch <= 'z') {
//				s += ch;
//				input += ch;
//				//system("cls");
//				cout << input << endl;
//				pInputStep->InputStep(s);
//				vecSegRes = pInputStep->GetTransRes();
//				for (int i = 0; i < vecSegRes.size(); i++) {
//					cout << vecSegRes[i] << endl;
//				}
//			}
//		}
//		
//	}
//	
//	return 0;
//
//	//cout << "hello" << endl;
// //   ofstream fout("log.txt");
// //   fout << "start process" << endl;
// //   fout.close();
//
// //   CHPYTable *cptable = new CHPYTable();
// //   //CSegment *segment = new CSegment(cptable->py);
//	//CStepInput *segment = new CStepInput(cptable->py);
//
// //   TranslateTree *trans = new TranslateTree();
//
// //   vector<vector<string>> segResult;
// //   vector<vector<string>> candidateCH;
// //   vector<string> temp;
// //   vector<YJNode*> translateResult;
// //   vector<pair<string, double>> output;
// //   vector<vector<pair<string, double>>> step;
//
// //   string input;
// //   cout << "input pinyin" << endl;
// //   char ch;
// //   string s;
// //   while (1){
// //       
//	//	while (ch = _getch()){
//	//		s = "";
//	//		if (27 == ch) {
//	//			return 0;
//	//		}
//	//		else if (8 == ch) {
//	//			if (input.size() > 0) {
//	//				input = input.substr(0, input.size() - 1);
//	//				trans->DeleteYJ(segment->GetSegment());
//	//				segment->DeleteStep();
//	//				system("cls");
//	//				cout << input << endl;
//	//				segment->output();
//	//				step.pop_back();
//	//				if (step.size() > 0) {
//	//					for (int i = 0; i < step.back().size(); i++) {
//	//						cout << UTF8ToGBK(step.back()[i].first) << endl;
//	//					}
//	//				}
//	//				segment->Log();
//	//				trans->ShowTreePath();
//	//			}
//	//			else {
//	//				input = "";
//	//				system("cls");
//	//			}
//	//		}
//	//		else if ((ch >= 'a' && ch <= 'z') || (ch > '1' && ch <= '9')) {
//	//			s += ch;
//	//			input += ch;
//	//			system("cls");
//	//			cout << input << endl;
//
//	//			candidateCH.clear();
//	//			output.clear();
//
//	//			SYSTEMTIME sys;
//	//			GetLocalTime(&sys);
//	//			printf("%4d/%02d/%02d %02d:%02d:%02d.%03d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
//	//			cout << endl;
//
//	//			segment->AddStep(s);
//
//	//			segResult = segment->GetSegment();
//
//
//
//	//			for (int i = 0; i < segResult.size(); i++) {
//	//				temp = segment->getpy(segResult[i].back());
//	//				temp = cptable->getChinese(temp);
//	//				candidateCH.push_back(temp);
//	//			}
//
//
//
//	//			translateResult = trans->getTranslate(segResult, candidateCH);
//
//	//			GetLocalTime(&sys);
//	//			printf("%4d/%02d/%02d %02d:%02d:%02d.%03d", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);
//	//			cout << endl;
//	//			cout << endl;
//
//	//			trans->ShowTreePath();
//	//			//     segment->output();
//	//			segment->Log();
//
//	//			int flag = 0;
//	//			for (int i = 0; i < translateResult.size(); i++) {
//	//				translateResult[i]->print();
//	//				for (int j = 0; j < translateResult[i]->trans_Total.size(); j++) {
//	//					if (translateResult[i]->trans_Total[j]->minSeg == 0) {
//	//						continue;
//	//					}
//	//					if (translateResult[i]->trans_Total[j]->minSeg == 1) {
//	//						output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res,
//	//							translateResult[i]->trans_Total[j]->prob));
//	//						flag = 2;
//	//					}
//	//					else {
//	//						if (flag == 0) {
//	//							output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res,
//	//								translateResult[i]->trans_Total[j]->prob));
//	//							flag = 1;
//	//						}
//	//						else if (flag == 1 && j == 0) {
//	//							if (translateResult[i]->trans_Total[j]->prob < output.back().second) {
//	//								output.pop_back();
//	//								output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res,
//	//									translateResult[i]->trans_Total[j]->prob));
//	//							}
//	//						}
//	//					}
//	//				}
//	//			}
//	//			sort(output.begin(), output.end(), cmpYJNode);
//	//			for (int i = 0; i < output.size(); i++) {
//	//				cout << UTF8ToGBK(output[i].first) << endl;
//	//			}
//	//			cout << endl;
//	//			step.push_back(output);
//	//		}
//
//	//	}
//
// //   }
// //   return 0;
//}