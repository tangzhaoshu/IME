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
#include "TransNine.cpp"
//#include "PCtable.cpp"
//#include "PYsegment.cpp"
//#include "translate.cpp"
//#include "PYSegNine.cpp"

using namespace std;


void TestNew() {
    fstream fin("inputNine.txt");
    if(!fin) {
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

    for (int i = 0; i < testData.size(); i ++) {
        cout << testData[i] << endl;
        for (int j = 0; j < testData[i].size(); j ++) {
            s = "";
            decodeNum ++;
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
        while(!pInputStep->IsEmpty()) {
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
    for (int i = 0; i < decodeRes.size(); i ++) {
        fout << decodeRes[i] << endl;
        delete[] decodeRes[i];
    }
    fout.close();
    delete pInputStep;
}






int main() {
 //   map<string, string> pyid = getpinyin();
 //   set<string> py;
 //   for (auto ite = pyid.begin(); ite != pyid.end(); ite ++) {
 //       py.insert(ite->first);
 //   }

//    CHPYTable *cptable = new CHPYTable();
//    CSegment *segment = new CSegment(py);
//    TranslateTree *trans = new TranslateTree();
//    CStepInput *segmentNine = new CStepInput(py);
//    TestFull(segment, trans, pyid);
    TestNew();
    //TestNine(segmentNine, trans, pyid);
    return 0;
}