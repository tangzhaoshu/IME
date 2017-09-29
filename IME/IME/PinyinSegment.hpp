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


class CSegNode{
public:
	vector<string> mSegRes;    // 切分路径
	PYNode* mLastPoint;         //当前切分路径的最后节点
	int legal;                  //路径的合法情况，用于判断有全拼无简拼规则
	int spellPart;          //简拼个数

	CSegNode() {
		mLastPoint = NULL;
		legal = 1;
		spellPart = 0;
	}

	~CSegNode() {
		mLastPoint = NULL;
		delete mLastPoint;
	}
};


//二十六键音节切分
class CSegment{
public:
	vector<vector<CSegNode*>> step;
	PYTree *pytree;
	CSegment() {
		pytree = new PYTree();
	}

	~CSegment() {
		delete pytree;
		while (step.size() != 0) {
			DeleteStep();
		}
	}

	void AddStep(string str) {
		PYNode *cur = pytree->root;
		PYNode *last;
		vector<CSegNode*> segtemp;
		CSegNode *segNodeTemp;
		if (step.size() == 0) {
			if (!cur->next[str[0] - 'a']) {
				cout << "input error" << endl;
				return;
			}
			else {
				segNodeTemp = new CSegNode();
				last = cur->next[str[0] - 'a'];
				segNodeTemp->mSegRes.push_back(str);
				segNodeTemp->mLastPoint = last;
			}
			segtemp.push_back(segNodeTemp);
			step.push_back(segtemp);
		}
		else {
			vector<CSegNode*> curSegment = step[step.size() - 1];
			for (int i = 0; i < curSegment.size(); i++) {
				if (curSegment[i]->legal == 1) {
					if (!curSegment[i]->mLastPoint->next[str[0] - 'a']) {
						if (cur->next[str[0] - 'a']) {
							segNodeTemp = new CSegNode();
							last = cur->next[str[0] - 'a'];
							segNodeTemp->mSegRes = curSegment[i]->mSegRes;
							segNodeTemp->mSegRes.push_back(str);
							segNodeTemp->mLastPoint = last;
							segtemp.push_back(segNodeTemp);
						}
					}
					else {
						if (curSegment[i]->mLastPoint->next[str[0] - 'a']) {
							segNodeTemp = new CSegNode();
							last = curSegment[i]->mLastPoint->next[str[0] - 'a'];
							segNodeTemp->mSegRes = curSegment[i]->mSegRes;
							string s = segNodeTemp->mSegRes.back();
							segNodeTemp->mSegRes.pop_back();
							segNodeTemp->mSegRes.push_back(s + str);
							segNodeTemp->mLastPoint = last;
							segtemp.push_back(segNodeTemp);
						}
						if (curSegment[i]->mLastPoint->flag == 1) {
							if (cur->next[str[0] - 'a']) {
								segNodeTemp = new CSegNode();
								last = cur->next[str[0] - 'a'];
								segNodeTemp->mSegRes = curSegment[i]->mSegRes;
								segNodeTemp->mSegRes.push_back(str);
								segNodeTemp->mLastPoint = last;
								if (last->flag == 0) {
									segNodeTemp->legal = 0;
								}
								segtemp.push_back(segNodeTemp);
							}

						}
					}
				}
				else {
					if (curSegment[i]->mLastPoint->next[str[0] - 'a']) {

						if (curSegment[i]->mLastPoint->next[str[0] - 'a']) {
							segNodeTemp = new CSegNode();
							last = curSegment[i]->mLastPoint->next[str[0] - 'a'];
							segNodeTemp->mSegRes = curSegment[i]->mSegRes;
							string s = segNodeTemp->mSegRes.back();
							segNodeTemp->mSegRes.pop_back();
							segNodeTemp->mSegRes.push_back(s + str);
							segNodeTemp->mLastPoint = last;
							if (last->flag == 0) {
								segNodeTemp->legal = 0;
							}
							segtemp.push_back(segNodeTemp);
						}

					}
				}

			}
			step.push_back(segtemp);
		}
	}

	void DeleteStep() {
		if (step.size() == 0) {
			return;
		}
		vector<CSegNode*> curSegment = step[step.size() - 1];
		for (int i = 0; i < curSegment.size(); i++) {
			delete curSegment[i];
		}
		step.pop_back();
	}

	vector<string> getpy(string str) {
		return pytree->getpy(str);
	}

	void clear() {
		if (step.size() == 0) {
			return;
		}
		while (step.size() != 0) {
			DeleteStep();
		}
	}

	void output() {
		if (step.size() == 0) {
			cout << "Please input string" << endl;
			return;
		}

		vector<CSegNode*> curSegment = step[step.size() - 1];
		cout << "       segment result as follow:     " << endl;
		for (int i = 0; i < curSegment.size(); i++) {
			if (curSegment[i]->legal == 1) {
				for (int j = 0; j < curSegment[i]->mSegRes.size(); j++) {
					cout << curSegment[i]->mSegRes[j] << " ";
				}
				cout << endl;
			}
		}
	}

	vector<vector<string>> GetSegment() {
		vector<vector<string>> result;
		if (step.size() == 0) {
			cout << "Please input string" << endl;
			return result;
		}
		vector<string> temp;
		vector<CSegNode*> curSegment = step[step.size() - 1];
		for (int i = 0; i < curSegment.size(); i++) {
			if (curSegment[i]->legal == 1) {
				for (int j = 0; j < curSegment[i]->mSegRes.size(); j++) {
					temp.push_back(curSegment[i]->mSegRes[j]);
				}
				result.push_back(temp);
				temp.clear();
			}
		}
		return result;
	}

	map<char*, char*> GetNewResult() {
		vector<vector<string>> vecSegment = GetSegment();
		map<char*, char*> result;
		char* history;
		char* newInput;
		string str = "";
		for (int i = 0; i < vecSegment.size(); i++) {
			str = "";
			for (int j = 0; j < vecSegment[i].size() - 1; j++) {
				str = str + vecSegment[i][j];
			}
			if (str == "") {
				history = NULL;
			}
			else{
				history = new char[str.length() + 1];
				strcpy_s(history, str.length() + 1, str.c_str());
			}
			str = vecSegment[i][vecSegment[i].size() - 1];
			newInput = new char[str.length() + 1];
			strcpy_s(newInput, str.length() + 1, str.c_str());
			result[history] = newInput;
		}
		return result;
	}

	void Log() {
		vector<vector<string>> result = GetSegment();
		ofstream fout("logNew.txt", ofstream::app);
		fout << "---------------------音节切分-------------------------" << endl;
		for (int i = 0; i < result.size(); i++) {
			fout << "切分线路" << i << ": ";
			for (int j = 0; j < result[i].size(); j++) {
				fout << result[i][j] << "'";
			}
			fout << endl;
		}
		fout << "---------------------音节切分-------------------------" << endl;
		fout << endl;
		fout.close();
	}
};



class CSegNine{
public:
	vector<CSegNode*> mTotalSeg;       //当前输入的所有切分路径
	PYTree *pytree;
	int MinSegNum;              //当前的最小切分个数
	int mMinSpell;          //当前切分结果中的最小简拼个数
	CSegNine() {
		pytree = new PYTree();
		MinSegNum = 0;
		mMinSpell = -1;
	}

	CSegNine(CSegNine *c) {
		MinSegNum = c->MinSegNum;
		mTotalSeg = c->mTotalSeg;
		pytree = c->pytree;
		mMinSpell = -1;
	}

	~CSegNine() {
		pytree = NULL;
		delete pytree;
		for (int i = 0; i < mTotalSeg.size(); i++) {
			delete mTotalSeg[i];
		}
	}

	void Filter() {
		if (mTotalSeg.size() <= 512) {
			return;
		}
		int left = 0;
		int right = mTotalSeg.size() - 1;
		CSegNode* pTempNode;
		while (left < right) {
			while (left < right && mTotalSeg[left]->mSegRes.size() == MinSegNum) {
				left++;
			}
			while (left < right && mTotalSeg[right]->mSegRes.size() == MinSegNum + 1) {
				right--;
			}
			if (left < right) {
				pTempNode = mTotalSeg[left];
				mTotalSeg[left] = mTotalSeg[right];
				mTotalSeg[right] = pTempNode;
				pTempNode = NULL;
			}
		}
		for (int i = mTotalSeg.size() - 1; i >= 512; i--) {
			delete mTotalSeg[i];
			mTotalSeg.pop_back();
		}
	}

	vector<string> getChar(string input) {
		vector<string> inputchar;
		if (input[0] < '2' || input[0] > '9') {
			return inputchar;
		}
		switch (input[0]) {
		case 50:
			inputchar = { "a", "b", "c" };
			break;
		case 51:
			inputchar = { "d", "e", "f" };
			break;
		case 52:
			inputchar = { "g", "h", "i" };
			break;
		case 53:
			inputchar = { "j", "k", "l" };
			break;
		case 54:
			inputchar = { "m", "n", "o" };
			break;
		case 55:
			inputchar = { "p", "q", "r", "s" };
			break;
		case 56:
			inputchar = { "t", "u", "v" };
			break;
		case 57:
			inputchar = { "w", "x", "y", "z" };
			break;
		}
		return inputchar;
	}

	//切分路径curSegment 对于输入字符str的切分结果。
	vector<CSegNode*> InsertChar(CSegNode *curSegment, string str) {
		vector<CSegNode*> segtemp;
		PYNode *cur = pytree->root;
		CSegNode *nineTemp;
		PYNode *last;
		if (curSegment->legal == 1) {
			if (!curSegment->mLastPoint->next[str[0] - 'a']) {
				if (cur->next[str[0] - 'a']) {
					nineTemp = new CSegNode();
					last = cur->next[str[0] - 'a'];
					nineTemp->mSegRes = curSegment->mSegRes;
					nineTemp->mSegRes.push_back(str);
					nineTemp->mLastPoint = last;
					if (curSegment->mLastPoint->flag == 1) {
						nineTemp->spellPart = curSegment->spellPart;
					}
					else {
						nineTemp->spellPart = curSegment->spellPart + 1;
					}
					segtemp.push_back(nineTemp);
				}
			}
			else {
				if (curSegment->mLastPoint->next[str[0] - 'a']) {
					nineTemp = new CSegNode();
					last = curSegment->mLastPoint->next[str[0] - 'a'];
					nineTemp->mSegRes = curSegment->mSegRes;
					string s = nineTemp->mSegRes.back();
					nineTemp->mSegRes.pop_back();
					nineTemp->mSegRes.push_back(s + str);
					nineTemp->mLastPoint = last;
					nineTemp->spellPart = curSegment->spellPart;
					segtemp.push_back(nineTemp);
				}
				if (curSegment->mLastPoint->flag == 1) {
					if (cur->next[str[0] - 'a']) {
						nineTemp = new CSegNode();
						last = cur->next[str[0] - 'a'];
						nineTemp->mSegRes = curSegment->mSegRes;
						nineTemp->mSegRes.push_back(str);
						nineTemp->mLastPoint = last;
						nineTemp->spellPart = curSegment->spellPart;
						if (last->flag == 0) {
							nineTemp->legal = 0;
						}
						segtemp.push_back(nineTemp);
					}
				}
			}
		}
		else {
			if (curSegment->mLastPoint->next[str[0] - 'a']) {
				if (curSegment->mLastPoint->next[str[0] - 'a']) {
					nineTemp = new CSegNode();
					last = curSegment->mLastPoint->next[str[0] - 'a'];
					nineTemp->mSegRes = curSegment->mSegRes;
					string s = nineTemp->mSegRes.back();
					nineTemp->mSegRes.pop_back();
					nineTemp->mSegRes.push_back(s + str);
					nineTemp->mLastPoint = last;
					nineTemp->spellPart = curSegment->spellPart;
					if (last->flag == 0) {
						nineTemp->legal = 0;
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
			for (int i = 0; i < inputchar.size(); i++) {
				if (!cur->next[inputchar[i][0] - 'a']) {
					continue;
				}
				CSegNode *temp = new CSegNode();
				temp->mSegRes.push_back(inputchar[i]);
				temp->mLastPoint = cur->next[inputchar[i][0] - 'a'];
				mTotalSeg.push_back(temp);
			}
			MinSegNum = 1;
			return;
		}
		MinSegNum = 0;
		vector<CSegNode*> segmentRes;
		vector<CSegNode*> segtemp;
		for (int i = 0; i < inputchar.size(); i++) {
			for (int j = 0; j < mTotalSeg.size(); j++) {
				segtemp = InsertChar(mTotalSeg[j], inputchar[i]);
				for (auto ite = segtemp.begin(); ite != segtemp.end(); ite++) {
					segmentRes.push_back(*ite);
					if (MinSegNum == 0) {
						MinSegNum = (*ite)->mSegRes.size();
					}
					else {
						if ((*ite)->mSegRes.size() < MinSegNum) {
							MinSegNum = (*ite)->mSegRes.size();
						}
					}
					if (mMinSpell == -1) {
						mMinSpell = (*ite)->spellPart;
					}
					else {
						if (mMinSpell > (*ite)->spellPart) {
							mMinSpell = (*ite)->spellPart;
						}
					}
				}
			}
		}
		mTotalSeg.clear();
		for (int i = 0; i < segmentRes.size(); i++) {
			if (segmentRes[i]->mSegRes.size() < MinSegNum + 2) {
				if (mMinSpell == segmentRes[i]->spellPart) {
					mTotalSeg.push_back(segmentRes[i]);
				}
				else {
					delete segmentRes[i];
					segmentRes[i] = NULL;
				}
			}
			else {
				delete segmentRes[i];
				segmentRes[i] = NULL;
			}
		}
	}
};


//九键音节切分
class CStepInput {
public:
	vector<CSegNine*> step;
	CSegNine *origin;
	string m_sInputNum;

	CStepInput() {
		m_sInputNum = "";
		origin = new CSegNine();
	}
	vector<string> getpy(string str) {
		return origin->pytree->getpy(str);
	}

	void AddStep(string str) {
		m_sInputNum += str;
		CSegNine *seg;
		if (step.size() == 0) {
			seg = new CSegNine(origin);
			seg->InputNum(str);
			step.push_back(seg);
			return;
		}
		seg = new CSegNine(step.back());
		seg->InputNum(str);
		seg->Filter();
		step.push_back(seg);
	}

	vector<vector<string>> GetSegment() {
		vector<vector<string>> result;
		if (step.size() == 0) {
			cout << "Please input string" << endl;
			return result;
		}
		CSegNine *curSeg = step.back();
		for (int i = 0; i < curSeg->mTotalSeg.size(); i++) {
			if (curSeg->mTotalSeg[i]->legal == 1 && curSeg->mTotalSeg[i]->mLastPoint->flag == 1) {
				result.push_back(curSeg->mTotalSeg[i]->mSegRes);
			}
		}
		if (result.size() == 0) {
			for (int i = 0; i < curSeg->mTotalSeg.size(); i++) {
				if (curSeg->mTotalSeg[i]->legal == 1) {
					result.push_back(curSeg->mTotalSeg[i]->mSegRes);
				}
			}
		}
		return result;
	}

	map<string, vector<char*>> GetNewResult() {
		vector<vector<string>> vecSegment = GetSegment();
		map<string, vector<char*>> result;
		string history;
		char* newInput;
		string str = "";
		for (int i = 0; i < vecSegment.size(); i++) {
			str = "";
			for (int j = 0; j < vecSegment[i].size() - 1; j++) {
				str = str + vecSegment[i][j];
			}
			if (str == "") {
				history = "";
			}
			else{
				history = m_sInputNum.substr(0, str.size());
			}
			str = vecSegment[i][vecSegment[i].size() - 1];
			newInput = new char[str.length() + 1];
			strcpy_s(newInput, str.length() + 1, str.c_str());
			result[history].push_back(newInput);
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
		for (int i = 0; i < curSeg->mTotalSeg.size(); i++) {
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
		m_sInputNum = m_sInputNum.substr(0, m_sInputNum.size() - 1);
	}

	void ClearAll(){
		while (step.size() != 0) {
			delete step.back();
			step.pop_back();
		}
	}

	void output() {
		if (step.size() == 0) {
			cout << "No Input" << endl;
			return;
		}
		vector<vector<string>> curSegment = GetSegment();
		cout << "---------segment result as follow--------------" << endl;
		for (int i = 0; i < curSegment.size(); i++) {
			for (int j = 0; j < curSegment[i].size(); j++) {
				cout << curSegment[i][j] << " ";
			}
			cout << endl;
		}
	}

	void Log() {
		vector<vector<string>> result = GetLogSegment();
		ofstream fout("logNew.txt", ofstream::app);
		fout << "---------------------音节切分-------------------------" << endl;
		for (int i = 0; i < result.size(); i++) {
			fout << "切分线路" << i << ": ";
			for (int j = 0; j < result[i].size(); j++) {
				fout << result[i][j] << "'";
			}
			fout << endl;
		}
		fout << "---------------------音节切分-------------------------" << endl;
		fout << endl;
		fout.close();
	}

	~CStepInput() {
		for (int i = 0; i < step.size(); i++) {
			delete step[i];
		}
		//delete origin->pytree;
	}

};
