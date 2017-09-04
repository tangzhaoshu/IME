#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <stack>
#include <conio.h>
#include <fstream>
#include <windows.h>
#include "pyTrie.hpp"
using namespace std;

class CNineNode{
public:
    vector<string> mSegRes;
    PYNode* mLastPoint;
    int legal;
    int state;

    CNineNode() {
        mLastPoint = NULL;
        legal = 1;
        state = 0;
    }

    ~CNineNode() {
        mLastPoint = NULL;
        delete mLastPoint;
    }
};

class CSegNine{
public:
    vector<CNineNode*> mFullSpell;
    vector<CNineNode*> mTotalSeg;
    PYTree *pytree;
    int MinSegNum;
    int mCurState;
    CSegNine(set<string> py) {
        pytree = new PYTree(py);
        MinSegNum = 0;
        mCurState = 0;
    }

    CSegNine(CSegNine *c) {
        MinSegNum = c->MinSegNum;
        mFullSpell = c->mFullSpell;
        mTotalSeg = c->mTotalSeg;
        pytree = c->pytree;
        mCurState = c->mCurState;
    }

    ~CSegNine() {
        pytree = NULL;
        delete pytree;
        for(int i = 0; i < mTotalSeg.size(); i ++) {
            delete mTotalSeg[i];
        }
    }

    vector<string> getChar(string input) {
        vector<string> inputchar;
        if (input[0] < '2' || input[0] > '9') {
            return inputchar;
        }
        switch (input[0]) {
            case 50:
                inputchar = {"a", "b", "c"};
                break;
            case 51:
                inputchar = {"d", "e", "f"};
                break;
            case 52:
                inputchar = {"g", "h", "i"};
                break;
            case 53:
                inputchar = {"j", "k", "l"};
                break;
            case 54:
                inputchar = {"m", "n", "o"};
                break;
            case 55:
                inputchar = {"p", "q", "r", "s"};
                break;
            case 56:
                inputchar = {"t", "u", "v"};
                break;
            case 57:
                inputchar = {"w", "x", "y", "z"};
                break;
        }
        return inputchar;
    }

    vector<CNineNode*> InsertChar(CNineNode *curSegment, string str) {
        vector<CNineNode*> segtemp;
        PYNode *cur = pytree->root;
        CNineNode *nineTemp;
        PYNode *last;
        if (curSegment->legal == 1) {
            if (!curSegment->mLastPoint->next[str[0] - 'a']) {
                if (mCurState == curSegment->state && cur->next[str[0] - 'a']) {
                    nineTemp = new CNineNode();
                    last = cur->next[str[0] - 'a'];
                    nineTemp->mSegRes = curSegment->mSegRes;
                    nineTemp->mSegRes.push_back(str);
                    nineTemp->mLastPoint = last;
                    if (last->flag == 1) {
                        nineTemp->state = 1;
                    } else {
                        if (curSegment->state == 1) {
                            nineTemp->state = 0;
                        } else {
                            nineTemp->state = -1;
                        }
                    }
                    segtemp.push_back(nineTemp);
                }
            } else {
                if(curSegment->mLastPoint->next[str[0] - 'a']) {
                    nineTemp = new CNineNode();
                    last = curSegment->mLastPoint->next[str[0] - 'a'];
                    nineTemp->mSegRes = curSegment->mSegRes;
                    string s = nineTemp->mSegRes.back();
                    nineTemp->mSegRes.pop_back();
                    nineTemp->mSegRes.push_back(s + str);
                    nineTemp->mLastPoint = last;
                    if (last->flag == 1) {
                        nineTemp->state = 1;
                    } else {
                        nineTemp->state = 0;
                    }
                    segtemp.push_back(nineTemp);
                }
                if (curSegment->mLastPoint->flag == 1) {
                    if (cur->next[str[0] - 'a']) {
                        nineTemp = new CNineNode();
                        last = cur->next[str[0] - 'a'];
                        nineTemp->mSegRes = curSegment->mSegRes;
                        nineTemp->mSegRes.push_back(str);
                        nineTemp->mLastPoint = last;
                        if (last->flag == 0) {
                            nineTemp->legal = 0;
                        }
                        segtemp.push_back(nineTemp);
                    }
                }
            }
        } else {
            if (curSegment->mLastPoint->next[str[0] - 'a']) {

                if (curSegment->mLastPoint->next[str[0] - 'a']) {
                    nineTemp = new CNineNode();
                    last = curSegment->mLastPoint->next[str[0] - 'a'];
                    nineTemp->mSegRes = curSegment->mSegRes;
                    string s = nineTemp->mSegRes.back();
                    nineTemp->mSegRes.pop_back();
                    nineTemp->mSegRes.push_back(s + str);
                    nineTemp->mLastPoint = last;
                    if (last->flag == 0) {
                        nineTemp->legal = 0;
                    } else {
                        nineTemp->state = 1;
                    }
                    segtemp.push_back(nineTemp);
                }
            }
        }
        return segtemp;
    }

    void InputNum(string input) {
        PYNode *cur = pytree->root;

        if (input[0] < '2' || input[0] > '9') {
            return;
        }
        vector<string> inputchar = getChar(input);
        if (mTotalSeg.size() == 0) {
            for (int i = 0; i < inputchar.size(); i ++) {
                if (!cur->next[inputchar[i][0] - 'a']) {
                    cout << "input error" << endl;
                    return;
                }

                CNineNode *temp = new CNineNode();
                temp->mSegRes.push_back(inputchar[i]);
                temp->mLastPoint = cur->next[inputchar[i][0] - 'a'];
                if (inputchar[i][0] == 'a' || inputchar[i][0] == 'o' || inputchar[i][0] == 'e') {
                    mCurState = 1;
                    temp->state = 1;
                }
                mTotalSeg.push_back(temp);
            }
            MinSegNum = 1;
            mFullSpell = mTotalSeg;
            return;
        }
        MinSegNum = 0;
        int optState = -2;
        vector<CNineNode*> segmentRes;
        vector<CNineNode*> segtemp;
        for (int i = 0; i < inputchar.size(); i ++) {
            for (int j = 0; j < mTotalSeg.size(); j ++) {
                segtemp = InsertChar(mTotalSeg[j], inputchar[i]);
                for (auto ite = segtemp.begin(); ite != segtemp.end(); ite ++) {
                    segmentRes.push_back(*ite);
                    if ((*ite)->state > optState) {
                        optState = (*ite)->state;
                    }
                    if (MinSegNum == 0) {
                        MinSegNum = (*ite)->mSegRes.size();
                    } else {
                        if ((*ite)->mSegRes.size() < MinSegNum) {
                            MinSegNum = (*ite)->mSegRes.size();
                        }
                    }
                }
            }
        }
        mCurState = optState;
        mTotalSeg.clear();
        mFullSpell.clear();
        for (int i = 0; i < segmentRes.size(); i ++) {
    //        for (int j = 0; j < segmentRes[i]->mSegRes.size(); j ++) {
    //            cout << segmentRes[i]->mSegRes[j] << " ";
    //        }
    //        cout << segmentRes[i]->state;
    //        cout << endl;
            if (segmentRes[i]->mSegRes.size() < MinSegNum + 2) {
                if (mCurState - segmentRes[i]->state <= 1) {
                    mTotalSeg.push_back(segmentRes[i]);
                    if (segmentRes[i]->state == mCurState) {
                        mFullSpell.push_back(segmentRes[i]);
                    }
                    if (mTotalSeg.size() > 512) {
                        break;
                    }
                }

            }
        }
    //    cout << "end" << endl;
        if (mFullSpell.size() == 0) {
            mFullSpell = mTotalSeg;
        }
    }
};

class CStepInput {
public:
    vector<CSegNine*> step;
    CSegNine *origin;

    CStepInput(set<string> py) {
        origin = new CSegNine(py);
    }

	vector<string> getpy(string str) {
		return origin->pytree->getpy(str);
	}

    void AddStep(string str) {
        CSegNine *seg;
        if (step.size() == 0) {
            seg = new CSegNine(origin);
            seg->InputNum(str);
            step.push_back(seg);
            return;
        }
        seg = new CSegNine(step.back());
        seg->InputNum(str);
        step.push_back(seg);
    }

    vector<vector<string>> GetSegment() {
        vector<vector<string>> result;
        if (step.size() == 0) {
            cout << "Please input string" << endl;
            return result;
        }
        CSegNine *curSeg = step.back();
        for (int i = 0; i < curSeg->mFullSpell.size(); i ++) {
            result.push_back(curSeg->mFullSpell[i]->mSegRes);
        }
        return result;
    }

    vector<vector<string>> GetLogSegment() {
        vector<vector<string>> result;
        if (step.size() == 0) {
            cout << "Please input string" << endl;
            return result;
        }
        CSegNine *curSeg = step.back();
        for (int i = 0; i < curSeg->mTotalSeg.size(); i ++) {
            result.push_back(curSeg->mTotalSeg[i]->mSegRes);
        }
        return result;
    }

    void DeleteStep() {
        if (step.size() == 0) {
            return;
        }
        delete step.back();
        step.pop_back();
    }

    void output() {
        if (step.size() == 0) {
            cout << "No Input" << endl;
            return;
        }
        vector<vector<string>> curSegment = GetSegment();
        cout << "---------segment result as follow--------------" << endl;
        for (int i = 0; i < curSegment.size(); i ++) {
            for (int j = 0; j < curSegment[i].size(); j ++) {
                cout << curSegment[i][j] << " ";
            }
            cout << endl;
        }
    }

    void Log() {
        vector<vector<string>> result = GetLogSegment();
        ofstream fout("log.txt", ofstream::app);
        fout << "---------------------音节切分-------------------------" << endl;
        for (int i = 0; i < result.size(); i ++) {
            fout << "切分线路" << i << ": ";
            for (int j = 0; j < result[i].size(); j ++) {
                fout << result[i][j] << "'";
            }
            fout << endl;
        }
        fout << "---------------------音节切分-------------------------" << endl;
        fout << endl;
        fout.close();
    }

    ~CStepInput() {
        for (int i = 0; i < step.size(); i ++) {
            delete step[i];
        }
        //delete origin->pytree;
    }

};
/*
int main() {
    ofstream fout("log.txt");
    fout << "开始程序" << endl;
    fout.close();

    CHPYTable *cptable = new CHPYTable();
    CStepInput *step = new CStepInput(cptable->py);
    string input;

    cout << "input pinyin" << endl;
    char ch;
    string s;
    while (1){
        s = "";
        while (!kbhit()){
        }
        ch = getch();
        if (27 == ch) {
            break;
        } else if (8 == ch) {
            if (input.size() > 0) {
                input = input.substr(0, input.size() - 1);
                step->DeleteStep();
                system("cls");
                cout << input << endl;
                step->output();
                continue;
            } else {
                input = "";
                system("cls");
                continue;
            }
        } else if (ch > '1' && ch <= '9') {
            s += ch;
            input += ch;
            system("cls");
            cout << input << endl;
            step->AddStep(s);
            step->output();
        }

    }
    return 0;
}
*/
