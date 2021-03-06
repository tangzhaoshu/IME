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
			pInputStep->InputStep(s);
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
			cout << UTF8ToGBK(pResult) << endl;
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

	ofstream fout("log.txt");
	for (int i = 0; i < decodeRes.size(); i++) {
		fout << decodeRes[i] << endl;
		delete[] decodeRes[i];
	}
	fout.close();
	delete pInputStep;
}

void Input() {
	ofstream fout("logTrans.txt");
	fout << "开始程序" << endl;
	fout.close();

	CStep* pInputStep = new CStep();

	string input;
	cout << "input pinyin" << endl;
	char ch;
	string s;
	vector<char*> vecSegRes;
	while (ch = _getch()){
		int line = 0;
		s = "";
		if (27 == ch) {
			break;
		}
		else if (8 == ch) {
			cout << 1;
			if (input.size() > 0) {
				input = input.substr(0, input.size() - 1);
				system("cls");
				cout << input << endl;
				pInputStep->DeleteStep();
				vecSegRes = pInputStep->GetTransRes();
				for (int i = 0; i < vecSegRes.size(); i++) {
					if (i >= line) {
						cout << endl;
						line += 20;
					}
					cout << UTF8ToGBK(vecSegRes[i]) << " ";
				}
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
			pInputStep->InputStep(s);
			vecSegRes = pInputStep->GetTransRes();
			for (int i = 0; i < vecSegRes.size(); i++) {
				if (i >= line) {
					cout << endl;
					line += 20;
				}
				cout << UTF8ToGBK(vecSegRes[i]) << " ";
			}
		}
		else if (ch >= '0' && ch <= '9') {
			s += ch;
			input += ch;
			system("cls");
			cout << input << endl;
			pInputStep->InputStepNine(s);
			vecSegRes = pInputStep->GetTransRes();
			for (int i = 0; i < vecSegRes.size(); i++) {
				if (i >= line) {
					cout << endl;
					line += 20;
				}
				cout << UTF8ToGBK(vecSegRes[i]) << " ";
			}
		}



	}
}

int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	TestNew();
//	TestNine();
//	Input();
	return 0;
}
