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
#include "TransTran.cpp"
//#include "PCtable.cpp"
//#include "PYsegment.cpp"
//#include "translate.cpp"
//#include "PYSegNine.cpp"

using namespace std;
/*
int cmpYJNode(const pair<string, double> &x, const pair<string, double> &y) {
    return x.second < y.second;
}

int stringToInt(string str) {
    if (str.size() == 0) {
        return 0;
    }
    int number = 0;
    for (int i = 0; i < str.size(); i ++) {
        int n = str[i] - '0';
        number = number * 10 + n;
    }
    return number;
}



map<string, string> getpinyin() {
    fstream fin("pysource.txt");
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    string str;
    string key;
    map<string, string> pyid;
    int index = 0;
    while(getline(fin, str)) {
        if (index == 0) {
            key = str;
            index ++;
        } else if (index == 1) {
            pyid[key] = str;
            index --;
        }
    }
    return pyid;
}

map<string, vector<pair<string, int>>> getchineseID() {
    fstream fin("chsource.txt");
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    string str;
    string key;
    string first;
    int index = 0;
    map<string, vector<pair<string, int>>> chid;
    while (getline(fin, str)) {
        if (index == 0) {
            key = str;
            index ++;
        } else if (index == 1) {
            first = str;
            index ++;
        } else if (index == 2) {
            chid[key].push_back(make_pair(first, stringToInt(str)));
            index = 0;
        }
    }
    return chid;
}

vector<string> getChinese(vector<string> pystr, map<string, string> pyID) {
    map<string, vector<pair<string, int>>> chID = getchineseID();
    vector<string> chstr;
    vector<pair<string, int>> chinfo;
    string id;
    for (int i = 0; i < pystr.size(); i ++) {
        id = pyID[pystr[i]];
        for (int j = 0; j < chID[id].size(); j ++) {
            chinfo.push_back(chID[id][j]);
        }
    }
    sort(chinfo.begin(), chinfo.end(), cmpYJNode);
    for (int i = 0; i < chinfo.size(); i ++) {
        chstr.push_back(chinfo[i].first);
    }
    return chstr;
}


void TestFull(CSegment *segment, TranslateTree *trans,  map<string, string> pyID) {
    fstream fin("inputShort.txt");
    if(!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    string str;
    vector<string> testData;
    vector<string> decodeRes;
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
    for (int i = 0; i < testData.size(); i ++) {
        cout << testData[i] << " " << testData[i].size() << endl;
        for (int j = 0; j < testData[i].size(); j ++) {
            s = "";
            decodeNum ++;
            s += testData[i][j];
            pInputStep->InputStep(s);
            vecSegRes = pInputStep->GetTransRes();
            decodeRes.push_back(vecSegRes[0]);
        }
        while(!pInputStep->IsEmpty()) {
            decodeNum ++;
            pInputStep->DeleteStep();
        }
    }
    DWORD dwUsed = GetTickCount() - dwStart;
    cout << dwUsed / decodeNum << endl;

    ofstream fout("log.txt");
    for (int i = 0; i < decodeRes.size(); i ++) {
        fout << decodeRes[i] << endl;
    }
    fout.close();


    vector<vector<string>> segResult;
    vector<vector<string>> candidateCH;
    vector<string> temp;
    vector<pair<string, double>> output;
    vector<YJNode*> translateResult;

    DWORD dwStart = GetTickCount();
    int decodeNum = 0;
    for (int i = 0; i < testData.size(); i ++) {
        cout << testData[i] << " " << testData[i].size() << endl;
        for (int j = 0; j < testData[i].size(); j ++) {
            string s = "";
            decodeNum ++;
            candidateCH.clear();
            s += testData[i][j];
            segment->InputChar(s);
            segResult = segment->GetSegment();
            
            for (int k = 0; k < segResult.size(); k ++) {
                temp = segment->pytree->getpy(segResult[k].back());
                temp = getChinese(temp, pyID);
                candidateCH.push_back(temp);
            }
            translateResult = trans->getTranslate(segResult, candidateCH);
        }
        int flag = 0;
        for (int i = 0; i < translateResult.size(); i ++) {
            for (int j = 0; j < translateResult[i]->trans_Total.size(); j ++) {
                if (translateResult[i]->trans_Total[j]->minSeg == 0) {
                    continue;
                }
                if (translateResult[i]->trans_Total[j]->minSeg == 1) {
                    if (flag != 2) {
                        output.clear();
                    }
                    output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res, 
                        translateResult[i]->trans_Total[j]->prob));
                    flag = 2;
                } else {
                    if (flag == 0) {
                        output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res, 
                        translateResult[i]->trans_Total[j]->prob));
                        flag = 1;
                    } else if (flag == 1 && j == 0) {
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
        decodeRes.push_back(output[0].first);
        output.clear();
        while (segment->step.size() != 0) {
            decodeNum ++;
            trans->DeleteYJ(segment->GetSegment());
            segment->DeleteChar();
        }
    }
    DWORD dwUsed = GetTickCount() - dwStart;
    cout << dwUsed / decodeNum << endl;

    ofstream fout("log.txt");
    for (int i = 0; i < decodeRes.size(); i ++) {
        fout << decodeRes[i] << endl;
    }
    fout.close();
}

void TestNine(CStepInput *segment, TranslateTree *trans,  map<string, string> pyID) {
    fstream fin("inputNine.txt");
    if(!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    string str;
    vector<string> testData;
    vector<string> decodeRes;
    while (getline(fin, str)) {
        testData.push_back(str);
    }
    fin.close();

    vector<vector<string>> segResult;
    vector<vector<string>> candidateCH;
    vector<string> temp;
    vector<pair<string, double>> output;
    vector<YJNode*> translateResult;

    DWORD dwStart = GetTickCount();
    int decodeNum = 0;

    for (int i = 0; i < testData.size(); i ++) {
        cout << testData[i] << "   " << i << endl;
        for (int j = 0; j < testData[i].size(); j ++) {
            string s = "";
            decodeNum ++;
            candidateCH.clear();
            s += testData[i][j];
            segment->AddStep(s);
            segResult = segment->GetSegment();
            for (int k = 0; k < segResult.size(); k ++) {
                temp = segment->origin->pytree->getpy(segResult[k].back());
                temp = getChinese(temp, pyID);
                candidateCH.push_back(temp);
            }
            translateResult = trans->getTranslate(segResult, candidateCH);
            
        }
        int flag = 0;
        for (int i = 0; i < translateResult.size(); i ++) {
            for (int j = 0; j < translateResult[i]->trans_Total.size(); j ++) {
                if (translateResult[i]->trans_Total[j]->minSeg == 0) {
                    continue;
                }
                if (translateResult[i]->trans_Total[j]->minSeg == 1) {
                    output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res, 
                        translateResult[i]->trans_Total[j]->prob));
                    flag = 2;
                } else {
                    if (flag == 0) {
                        output.push_back(make_pair(translateResult[i]->trans_Total[j]->trans_res, 
                        translateResult[i]->trans_Total[j]->prob));
                        flag = 1;
                    } else if (flag == 1 && j == 0) {
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
        decodeRes.push_back(output[0].first);
        output.clear();
        while (segment->step.size() != 0) {
            decodeNum ++;
            trans->DeleteYJ(segment->GetSegment());
            segment->DeleteStep();
        }
        DWORD dwUsed = GetTickCount() - dwStart;
        cout << dwUsed / decodeNum << endl;
    }
    DWORD dwUsed = GetTickCount() - dwStart;
    cout << dwUsed / decodeNum << endl;

    ofstream fout("log.txt");
    for (int i = 0; i < decodeRes.size(); i ++) {
        fout << decodeRes[i] << endl;
    }
    fout.close();
}
*/


void TestNew() {
    fstream fin("inputFull.txt");
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