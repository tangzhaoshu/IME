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
#include "BigramUP.hpp"
//#include "PYsegment.cpp"
#include "PYSegNine.hpp"
//#include "FunctionGroup.cpp"

using namespace std;



DWORD DWUSED = 0;
int SCREATE = 0;
int SDELETE = 0;

class CStateNode {
public:
	CBigramNode* m_pChinese;
	CStateNode* m_pNext;
	CStateNode* m_pParent;
	int m_nProb;
	int m_nSplit;

	CStateNode() {
		SCREATE++;
		m_pChinese = NULL;
		m_pNext = NULL;
		m_pParent = NULL;
		m_nProb = 0;
		m_nSplit = 0;
	}

	CStateNode(CBigramNode* p) {
		SCREATE++;
		m_pChinese = p;
		m_pNext = NULL;
		m_pParent = NULL;
		m_nProb = 0;
		m_nSplit = 0;
	}

	void SetProb(int p) {
		m_nProb = p;
	}

	void SetSplit(int p) {
		m_nSplit = p;
	}

	void SetNext(CStateNode* p) {
		m_pNext = p;
	}

	void SetChinese(CBigramNode* p) {
		m_pChinese = p;
	}

	bool operator < (CStateNode* p) {
		if (m_nSplit == 1 && p->m_nSplit != 1) {
			return 1;
		}
		if (m_nSplit != 1 && p->m_nSplit == 1) {
			return 0;
		}
		if (m_nProb < p->m_nProb) {
			return 1;
		}
		return 0;
	}

	~CStateNode() {
		SDELETE++;
		m_pChinese = NULL;
		m_pNext = NULL;
		m_pParent = NULL;
		delete m_pChinese;
		delete m_pNext;
		delete m_pParent;
	}
};

bool CompareState(CStateNode* p1, CStateNode* p2) {
	if (p1->m_nSplit == 1 && p2->m_nSplit != 1) {
		return 1;
	}
	if (p1->m_nSplit != 1 && p2->m_nSplit == 1) {
		return 0;
	}
	if (p1->m_nProb < p2->m_nProb) {
		return 1;
	}
	return 0;
}

class CChineseNode {
public:
	int m_nMinSplit;
	int m_nMinProb;
	char* m_pTransChinese;
	CStateNode* m_pLeftFinal;
	CStateNode* m_pLeftMid;

	CChineseNode() {
		m_nMinSplit = 0;
		m_nMinProb = 0;
		m_pTransChinese = NULL;
		m_pLeftFinal = NULL;
		m_pLeftMid = NULL;
	}

	~CChineseNode() {
		CStateNode* cur = m_pLeftFinal;
		CStateNode* temp;
		while (cur != NULL) {
			temp = cur;
			cur = cur->m_pNext;
			delete temp;
		}
		cur = m_pLeftMid;
		while (cur != NULL) {
			temp = cur;
			cur = cur->m_pNext;
			delete temp;
		}
		delete[] m_pTransChinese;
	}

	void DeleteState(CStateNode* p) {
		CStateNode* cur = m_pLeftFinal;
		if (cur == NULL) {
			cout << "Node not exist" << endl;
			return;
		}
		while (cur != NULL) {
			if (cur == p) {
				break;
			}
			cur = cur->m_pNext;
		}
		if (cur == NULL) {
			cout << "Node not exist" << endl;
			return;
		}
		if (cur->m_pParent == NULL) {
			m_pLeftFinal = cur->m_pNext;
			if (cur->m_pNext != NULL) {
				cur->m_pNext->m_pParent = NULL;
			}
		}
		else{
			cur->m_pParent->m_pNext = cur->m_pNext;
			if (cur->m_pNext != NULL) {
				cur->m_pNext->m_pParent = cur->m_pParent;
			}
		}
		delete p;
		cur = m_pLeftFinal;
		if (cur == NULL) {
			m_nMinProb = 0;
			m_nMinSplit = 0;
			return;
		}
		m_nMinProb = cur->m_nProb;
		m_nMinSplit = cur->m_nSplit;
	}

	void DeleteMidState(CStateNode* p) {
		CStateNode* cur = m_pLeftMid;
		if (cur == NULL) {
			cout << "Node not exist" << endl;
			return;
		}
		while (cur != NULL) {
			if (cur == p) {
				break;
			}
			cur = cur->m_pNext;
		}
		if (cur == NULL) {
			cout << "Node not exist" << endl;
			return;
		}
		if (cur->m_pParent == NULL) {
			m_pLeftMid = cur->m_pNext;
			if (cur->m_pNext != NULL) {
				cur->m_pNext->m_pParent = NULL;
			}
		}
		else{
			cur->m_pParent->m_pNext = cur->m_pNext;
			if (cur->m_pNext != NULL) {
				cur->m_pNext->m_pParent = cur->m_pParent;
			}
		}
		delete p;
	}

	void SetTrans(char* c) {
		m_pTransChinese = c;
	}

	void InsertFinal(CStateNode* c) {
		if (m_pLeftFinal == NULL) {
			m_pLeftFinal = c;
			m_nMinProb = c->m_nProb;
			m_nMinSplit = c->m_nSplit;
			return;
		}
		CStateNode* cur = m_pLeftFinal;
		if (cur->m_pChinese == c->m_pChinese) {
			if (CompareState(c, cur)) {
				m_pLeftFinal = c;
				c->m_pNext = cur->m_pNext;
				if (cur->m_pNext != NULL) {
					cur->m_pNext->m_pParent = c;
				}
				m_nMinProb = m_pLeftFinal->m_nProb;
				m_nMinSplit = m_pLeftFinal->m_nSplit;
				delete cur;
				return;
			}
			else {
				delete c;
				return;
			}
		}
		while (cur->m_pNext != NULL) {
			if (cur->m_pNext->m_pChinese == c->m_pChinese) {
				if (CompareState(c, cur->m_pNext)) {
					CStateNode* tempState = cur->m_pNext;
					cur->m_pNext = c;
					c->m_pParent = cur;
					c->m_pNext = tempState->m_pNext;
					if (tempState->m_pNext != NULL) {
						tempState->m_pNext->m_pParent = c;
					}
					delete tempState;
					m_nMinProb = m_pLeftFinal->m_nProb;
					m_nMinSplit = m_pLeftFinal->m_nSplit;
					return;
				}
				else {
					delete c;
					return;
				}
			}
			cur = cur->m_pNext;
		}
		cur = m_pLeftFinal;
		if (cur == NULL) {
			m_pLeftFinal = c;
			m_nMinProb = m_pLeftFinal->m_nProb;
			m_nMinSplit = m_pLeftFinal->m_nSplit;
			return;
		}
		if (CompareState(c, cur)) {
			m_pLeftFinal = c;
			c->m_pNext = cur;
			cur->m_pParent = c;
			m_nMinProb = m_pLeftFinal->m_nProb;
			m_nMinSplit = m_pLeftFinal->m_nSplit;
			return;
		}
		while (cur->m_pNext != NULL) {
			if (CompareState(cur->m_pNext, c)) {
				cur = cur->m_pNext;
			}
			else {
				break;
			}
		}
		if (cur->m_pNext == NULL) {
			cur->m_pNext = c;
			c->m_pParent = cur;
		}
		else {
			CStateNode* tempState = cur->m_pNext;
			tempState->m_pParent = c;
			cur->m_pNext = c;
			c->m_pNext = tempState;
			c->m_pParent = cur;
		}
		m_nMinProb = m_pLeftFinal->m_nProb;
		m_nMinSplit = m_pLeftFinal->m_nSplit;
	}

	void InsertMid(CStateNode* c) {
		CStateNode* cur = m_pLeftMid;
		if (cur == NULL) {
			m_pLeftMid = c;
			return;
		}
		if (cur->m_pChinese == c->m_pChinese) {
			if (CompareState(cur, c)) {
				delete c;
				return;
			}
			else {
				m_pLeftMid = c;
				c->m_pNext = cur->m_pNext;
				if (cur->m_pNext != NULL) {
					cur->m_pNext->m_pParent = c;
				}
				delete cur;
				return;
			}
		}
		while (cur->m_pNext != NULL) {
			if (cur->m_pNext->m_pChinese == c->m_pChinese) {
				if (CompareState(cur->m_pNext, c)) {
					delete c;
					return;
				}
				else {
					CStateNode* tempState = cur->m_pNext;
					cur->m_pNext = c;
					c->m_pParent = cur;
					c->m_pNext = tempState->m_pNext;
					if (tempState->m_pNext != NULL) {
						tempState->m_pNext->m_pParent = c;
					}
					delete tempState;
					return;
				}
			}
			cur = cur->m_pNext;
		}
		cur->m_pNext = c;
		c->m_pParent = cur;
	}

	CStateNode* FindState(CStateNode* p) {
		CStateNode* cur = m_pLeftFinal;
		if (cur == NULL) {
			return NULL;
		}
		while (cur != NULL) {
			if (cur == p) {
				return cur;
			}
			cur = cur->m_pNext;
		}
		return NULL;
	}

	bool operator < (CChineseNode* p) {
		if (m_nMinSplit == 1 && p->m_nMinSplit != 1) {
			return 1;
		}
		if (m_nMinSplit != 1 && p->m_nMinSplit == 1) {
			return 0;
		}
		if (m_nMinProb < p->m_nMinProb) {
			return 1;
		}
		return 0;
	}

};

bool Compare(CChineseNode* p1, CChineseNode* p2) {
	if (p1->m_nMinSplit == 1 && p2->m_nMinSplit != 1) {
		return 1;
	}
	if (p1->m_nMinSplit != 1 && p2->m_nMinSplit == 1) {
		return 0;
	}
	if (p1->m_nMinProb < p2->m_nMinProb) {
		return 1;
	}
	return 0;
}


class CInputString {
public:
	char* m_pInputStr;
	CInputString* m_pNext;
	CChineseNode** m_pTotalRes;
	int m_nTranSize;
	int m_nCapacity;

	CInputString() {
		m_nTranSize = 0;
		m_nCapacity = 0;
		m_pInputStr = NULL;
		m_pNext = NULL;
		m_pTotalRes = NULL;
	}

	~CInputString() {
		delete[] m_pInputStr;
		m_pNext = NULL;
		delete m_pNext;

		for (int i = 0; i < m_nTranSize; i++) {
			delete m_pTotalRes[i];
		}
		delete[] m_pTotalRes;
	}

	void DeletePart(int pos) {
		if (pos >= m_nTranSize) {
			return;
		}
		for (int i = pos; i < m_nTranSize; i++) {
			delete m_pTotalRes[i];
		}
		m_nTranSize = pos;
	}



	int Partition(CChineseNode** Array, int left, int right) {
		int l = left;
		int r = right;
		CChineseNode* temp = Array[r];
		while (l < r) {
			while (r > l && Compare(Array[l], temp)) {
				l++;
			}
			if (l < r) {
				Array[r] = Array[l];
				r--;
			}
			while (r > l && Compare(temp, Array[r])) {
				r--;
			}
			if (l < r) {
				Array[l] = Array[r];
				l++;
			}
		}
		Array[l] = temp;
		return l;
	}

	void QuickSort(CChineseNode** Array, int left, int right) {
		if (right <= left) {
			return;
		}
		int pivot = (left + right) / 2;

		CChineseNode* temp;
		temp = Array[pivot];
		Array[pivot] = Array[right];
		Array[right] = temp;
		pivot = Partition(Array, left, right);
		QuickSort(Array, left, pivot - 1);
		QuickSort(Array, pivot + 1, right);
	}

	void SortTrans() {
		if (m_nTranSize == 0) {
			return;
		}
		int left = 0;
		int right = m_nTranSize - 1;
		int emptyNum = 0;
		while (left < right) {
			while (left < right && m_pTotalRes[left] != NULL) {
				left++;
			}
			while (left < right && m_pTotalRes[right] == NULL) {
				right--;
			}
			if (left < right) {
				m_pTotalRes[left] = m_pTotalRes[right];
				m_pTotalRes[right] = NULL;
				left++;
				right--;
				emptyNum++;
			}
		}
		if (m_pTotalRes[left] == NULL) {
			m_nTranSize = left;
		}
		else {
			m_nTranSize = left + 1;
		}
		QuickSort(m_pTotalRes, 0, m_nTranSize - 1);
	}

	void Filter() {
		int nLimit = 3000;
		if (m_nTranSize < 3000) {
			return;
		}
		for (int i = 3000; i < m_nTranSize; i++) {
			delete m_pTotalRes[i];
			m_pTotalRes[i] = NULL;
		}
		m_nTranSize = 3000;

	}

	void DeleteChinese(CChineseNode* p) {
		if (m_nTranSize == 0) {
			return;
		}
		for (int i = 0; i < m_nTranSize; i++) {
			if (p == m_pTotalRes[i]) {
				delete p;
				m_pTotalRes[i] = NULL;
			}
		}
	}

	void InsertChineseOld(CChineseNode* p) {
		if (p->m_pLeftFinal == NULL && p->m_pLeftMid == NULL) {
			delete p;
			return;
		}
		if (m_nTranSize < m_nCapacity) {
			m_pTotalRes[m_nTranSize] = p;
			m_nTranSize++;
			return;
		}
		if (m_nCapacity == 0) {
			m_nCapacity = 1;
		}
		else {
			m_nCapacity += m_nCapacity;
		}
		CChineseNode** arrayTemp = m_pTotalRes;
		m_pTotalRes = new CChineseNode*[m_nCapacity];
		for (int i = 0; i < m_nTranSize; i++) {
			m_pTotalRes[i] = arrayTemp[i];
			arrayTemp[i] = NULL;
		}
		delete[] arrayTemp;
		m_pTotalRes[m_nTranSize] = p;
		m_nTranSize++;
	}

	void JudegeChinese(CChineseNode* p) {
		CStateNode* curState = p->m_pLeftFinal;
		CStateNode* tempState = NULL;
		while (curState != NULL) {
			int flag = 0;
			for (int i = 0; i < m_nTranSize; i++) {
				if (m_pTotalRes[i] == NULL) {
					continue;
				}
				tempState = m_pTotalRes[i]->m_pLeftFinal;
				while (tempState != NULL) {
					if (tempState->m_pChinese == curState->m_pChinese) {
						break;
					}
					tempState = tempState->m_pNext;
				}
				if (tempState != NULL) {
					if (CompareState(curState, tempState)) {
						m_pTotalRes[i]->DeleteState(tempState);
						if (m_pTotalRes[i]->m_pLeftFinal == NULL && m_pTotalRes[i]->m_pLeftMid == NULL) {
							delete m_pTotalRes[i];
							m_pTotalRes[i] = NULL;
						}
						break;
					}
					else {
						flag = 1;
						tempState = curState;
						curState = curState->m_pNext;
						p->DeleteState(tempState);
						break;
					}
				}
			}
			if (flag == 0) {
				curState = curState->m_pNext;
			}
		}
		curState = p->m_pLeftMid;
		tempState = NULL;
		while (curState != NULL) {
			int flag = 0;
			for (int i = 0; i < m_nTranSize; i++) {
				if (m_pTotalRes[i] == NULL) {
					continue;
				}
				tempState = m_pTotalRes[i]->m_pLeftMid;
				while (tempState != NULL) {
					if (tempState->m_pChinese == curState->m_pChinese) {
						break;
					}
					tempState = tempState->m_pNext;
				}
				if (tempState != NULL) {
					if (CompareState(curState, tempState)) {
						m_pTotalRes[i]->DeleteMidState(tempState);
						if (m_pTotalRes[i]->m_pLeftFinal == NULL && m_pTotalRes[i]->m_pLeftMid == NULL) {
							delete m_pTotalRes[i];
							m_pTotalRes[i] = NULL;
						}
						break;
					}
					else {
						flag = 1;
						tempState = curState;
						curState = curState->m_pNext;
						p->DeleteMidState(tempState);
						break;
					}
				}
			}
			if (flag == 0) {
				curState = curState->m_pNext;
			}
		}
	}

	void InsertChinese(CChineseNode* p) {
		CStateNode* curState = p->m_pLeftFinal;
		CStateNode* tempState = NULL;
		while (curState != NULL) {
			int flag = 0;
			for (int i = 0; i < m_nTranSize; i++) {
				if (m_pTotalRes[i] == NULL) {
					continue;
				}
				tempState = m_pTotalRes[i]->m_pLeftFinal;
				while (tempState != NULL) {
					if (tempState->m_pChinese == curState->m_pChinese) {
						break;
					}
					tempState = tempState->m_pNext;
				}
				if (tempState != NULL) {
					if (CompareState(curState, tempState)) {
						m_pTotalRes[i]->DeleteState(tempState);
						if (m_pTotalRes[i]->m_pLeftFinal == NULL && m_pTotalRes[i]->m_pLeftMid == NULL) {
							delete m_pTotalRes[i];
							m_pTotalRes[i] = NULL;
						}
						break;
					}
					else {
						flag = 1;
						tempState = curState;
						curState = curState->m_pNext;
						p->DeleteState(tempState);
						break;
					}
				}
			}
			if (flag == 0) {
				curState = curState->m_pNext;
			}
		}
		curState = p->m_pLeftMid;
		tempState = NULL;
		while (curState != NULL) {
			int flag = 0;
			for (int i = 0; i < m_nTranSize; i++) {
				if (m_pTotalRes[i] == NULL) {
					continue;
				}
				tempState = m_pTotalRes[i]->m_pLeftMid;
				while (tempState != NULL) {
					if (tempState->m_pChinese == curState->m_pChinese) {
						break;
					}
					tempState = tempState->m_pNext;
				}
				if (tempState != NULL) {
					if (CompareState(curState, tempState)) {
						m_pTotalRes[i]->DeleteMidState(tempState);
						if (m_pTotalRes[i]->m_pLeftFinal == NULL && m_pTotalRes[i]->m_pLeftMid == NULL) {
							delete m_pTotalRes[i];
							m_pTotalRes[i] = NULL;
						}
						break;
					}
					else {
						flag = 1;
						tempState = curState;
						curState = curState->m_pNext;
						p->DeleteMidState(tempState);
						break;
					}
				}
			}
			if (flag == 0) {
				curState = curState->m_pNext;
			}
		}
		if (p->m_pLeftFinal == NULL && p->m_pLeftMid == NULL) {
			delete p;
			return;
		}
		if (m_nTranSize < m_nCapacity) {
			m_pTotalRes[m_nTranSize] = p;
			m_nTranSize++;
			return;
		}
		if (m_nCapacity == 0) {
			m_nCapacity = 1;
		}
		else {
			m_nCapacity += m_nCapacity;
		}
		CChineseNode** arrayTemp = m_pTotalRes;
		m_pTotalRes = new CChineseNode*[m_nCapacity];
		for (int i = 0; i < m_nTranSize; i++) {
			m_pTotalRes[i] = arrayTemp[i];
			arrayTemp[i] = NULL;
		}
		delete[] arrayTemp;
		m_pTotalRes[m_nTranSize] = p;
		m_nTranSize++;
	}
};

class CInputNumber {
public:
	CInputString** m_pTotalRes;
	vector<char*> m_vecResult;
	CInputNumber* m_pNext;
	int m_nCapacity;
	int m_nSize;

	CInputNumber() {
		m_pTotalRes = NULL;
		m_nSize = 0;
		m_nCapacity = 0;
		m_pNext = NULL;
	}
	~CInputNumber() {
		for (int i = 0; i < m_nSize; i++) {
			m_vecResult[i] = NULL;
		}
		m_pNext = NULL;
		for (int i = 0; i < m_nSize; i++) {
			m_pTotalRes[i] = NULL;
		}
		delete[] m_pTotalRes;
	}

	void Insert(CInputString* p) {
		if (m_nSize < m_nCapacity) {
			m_pTotalRes[m_nSize] = p;
			m_nSize++;
			return;
		}
		if (m_nCapacity == 0) {
			m_nCapacity = 1;
		}
		else {
			m_nCapacity += m_nCapacity;
		}
		CInputString** arrayTemp = m_pTotalRes;
		m_pTotalRes = new CInputString*[m_nCapacity];
		for (int i = 0; i < m_nSize; i++) {
			m_pTotalRes[i] = arrayTemp[i];
			arrayTemp[i] = NULL;
		}
		delete[] arrayTemp;
		m_pTotalRes[m_nSize] = p;
		m_nSize++;
	}

	void Judge(CChineseNode* p) {
		for (int i = 0; i < m_nSize; i++) {
			m_pTotalRes[i]->JudegeChinese(p);
		}
	}

};

class CStep {
public:
	CInputString* m_pHead;
	CInputNumber* m_pStep;
	CBigramTree* m_pBigramRoot;
	CStepInput* m_pSegment;
	map<string, string> m_sPinyinId;
	int m_nTag[500];

	CStep() {
		m_pStep = NULL;
		m_pHead = new CInputString();
		m_pBigramRoot = new CBigramTree();
		cout << "BigramTree Finish" << endl;
		m_pSegment = new CStepInput();
		cout << "Segment Finish" << endl;
		SetPYCH();
	}

	void SetPYCH() {
		fstream fin("pysource.txt");
		if (!fin) {
			cout << "open file error" << endl;
			exit(1);
		}
		string str;
		string key;
		int index = 0;
		while (getline(fin, str)) {
			if (index == 0) {
				key = str;
				index++;
			}
			else if (index == 1) {
				m_sPinyinId[key] = str;
				index--;
			}
		}
		fin.close();
	}

	void SetTag(vector<string> vecID) {
		for (int i = 0; i < vecID.size(); i++) {
			int id = StringToInt(m_sPinyinId[vecID[i]]);
			m_nTag[id - 100] = 1;
		}
	}

	void ClearTag() {
		for (int i = 0; i < 500; i++) {
			m_nTag[i] = 0;
		}
	}



	CInputString* FindHistory(char* c) {
		CInputString* cur = m_pHead;
		while (cur->m_pNext != NULL) {
			if (strcmp(cur->m_pNext->m_pInputStr, c) == 0) {
				return cur->m_pNext;
			}
			cur = cur->m_pNext;
		}
		return NULL;
	}

	CInputString* FindCurString(char* c) {
		if (m_pStep == NULL) {
			return NULL;
		}
		for (int i = 0; i < m_pStep->m_nSize; i++) {
			if (strcmp(m_pStep->m_pTotalRes[i]->m_pInputStr, c) == 0) {
				return m_pStep->m_pTotalRes[i];
			}
		}
		return NULL;
	}

	void DeleteInputString(CInputString* p) {

		CInputString* pStrTemp = m_pHead;
		while (pStrTemp->m_pNext != NULL) {
			if (pStrTemp->m_pNext == p) {
				pStrTemp->m_pNext = p->m_pNext;
				delete p;
				break;
			}
			pStrTemp = pStrTemp->m_pNext;
		}

	}


	set<char*> InputWithoutHis(CInputString* pCurString) {
		CStateNode* pState = NULL;
		CChineseNode* pChinese = NULL;
		CBigramNode** pBigramTotal;
		int nSize;
		set<char*> chinese;
		pBigramTotal = m_pBigramRoot->GetRoot()->m_pChild;
		nSize = m_pBigramRoot->GetRoot()->m_nSize;
		for (int i = 0; i < nSize; i++) {
			int flag = 0;
			if (i > 0 && strcmp(pBigramTotal[i]->m_pChinese, pBigramTotal[i - 1]->m_pChinese) != 0) {
				pChinese = NULL;
				flag = 1;
			}
			int pyTotal = pBigramTotal[i]->m_nID;
			int pyNum;
			while (pChinese == NULL && pyTotal != 0) {
				pyNum = pyTotal % 1000 - 100;
				if (m_nTag[pyNum] == 1) {
					chinese.insert(pBigramTotal[i]->m_pChinese);
					pChinese = new CChineseNode();
					pChinese->m_pTransChinese = GetSubStr(pBigramTotal[i]->m_pChinese, 0, strlen(pBigramTotal[i]->m_pChinese));
					break;
				}
				pyTotal = pyTotal / 1000;
			}
			if (pChinese != NULL) {
				//	cout << pBigramTotal[i]->m_pChinese << endl;
				if (pBigramTotal[i]->m_nState == 1) {
					pState = new CStateNode(pBigramTotal[i]);
					pState->SetSplit(1);
					pState->SetProb(pBigramTotal[i]->m_nTransPorb);
					pChinese->InsertFinal(pState);
				}
				else if (pBigramTotal[i]->m_nState == 0) {
					pState = new CStateNode(pBigramTotal[i]);
					pChinese->InsertMid(pState);
				}
				if (flag == 1){
					if (pChinese->m_pLeftFinal != NULL || pChinese->m_pLeftMid != NULL) {
						pCurString->JudegeChinese(pChinese);
						pCurString->InsertChineseOld(pChinese);
					}
					else {
						delete pChinese;
						pChinese = NULL;
					}
				}
			}

		}
		return chinese;
	}

	void InputJoint(CInputString* pCurString, CInputString* pLastInput, set<char*> chinese){
		CStateNode* pState = NULL;
		CChineseNode* pChinese = NULL;
		CStateNode* pLastState = NULL;
		CChineseNode* pLastChinese = NULL;
		CBigramNode** pBigramTotal = m_pBigramRoot->GetRoot()->m_pChild;;
		int nSize = m_pBigramRoot->GetRoot()->m_nSize;
		for (int i = 0; i < pLastInput->m_nTranSize; i++){
			pLastChinese = pLastInput->m_pTotalRes[i];
			pLastState = pLastChinese->m_pLeftFinal;
			if (pLastState != NULL) {
				break;
			}
		}
		vector<CChineseNode*> vecChineseBuff;
		for (int i = 0; i < nSize; i++) {
			int flag = 0;
			if (i > 0 && strcmp(pBigramTotal[i]->m_pChinese, pBigramTotal[i - 1]->m_pChinese) != 0) {
				pChinese = NULL;
				flag = 1;
			}
			int pyTotal = pBigramTotal[i]->m_nID;
			int pyNum;
			while (pChinese == NULL && pyTotal != 0) {
				pyNum = pyTotal % 1000 - 100;
				if (m_nTag[pyNum] == 1) {
					if (chinese.find(pBigramTotal[i]->m_pChinese) == chinese.end()) {
						pChinese = new CChineseNode();
						pChinese->m_pTransChinese = MergeStr(pLastChinese->m_pTransChinese, pBigramTotal[i]->m_pChinese);;
					}
					break;
				}
				pyTotal = pyTotal / 1000;
			}
			if (pChinese != NULL) {
				//	cout << pBigramTotal[i]->m_pChinese << pBigramTotal[i]->m_nState << endl;
				if (pBigramTotal[i]->m_nState == 1) {
					pState = new CStateNode(pBigramTotal[i]);
					pState->SetSplit(1);
					pState->SetProb(pLastState->m_nProb + pBigramTotal[i]->m_nTransPorb);
					pState->SetSplit(pLastState->m_nSplit + 1);
					pChinese->InsertFinal(pState);
				}
				else if (pBigramTotal[i]->m_nState == 0) {
					pState = new CStateNode(pBigramTotal[i]);
					pState->SetProb(pLastState->m_nProb);
					pState->SetSplit(pLastState->m_nSplit);
					pChinese->InsertMid(pState);
				}
				if (flag == 1) {
					if (pChinese->m_pLeftFinal != NULL || pChinese->m_pLeftMid != NULL) {
						vecChineseBuff.push_back(pChinese);
					}
					else {
						delete pChinese;
						pChinese = NULL;
					}
				}

			}
		}
		for (int k = 0; k < vecChineseBuff.size(); k++) {
			pCurString->InsertChinese(vecChineseBuff[k]);
		}
		vecChineseBuff.clear();
	}

	void InputStringNew(CInputString* pCurString, char* history) {
		CBigramNode* pBigramNode = NULL;
		CInputString* pLastInput = NULL;
		CStateNode* pState = NULL;
		CChineseNode* pChinese = NULL;
		CBigramNode** pBigramTotal;
		int nSize;
		set<char*> chinese;
		if (history != NULL && strlen(history) != 0) {
			pLastInput = FindHistory(history);
			if (pLastInput == NULL) {
				return;
			}
		}
		if (pLastInput == NULL) {
			chinese = InputWithoutHis(pCurString);
			return;
		}
		CChineseNode* pLastChinese = NULL;
		CStateNode* pLastState = NULL;
		for (int i = 0; i < pLastInput->m_nTranSize; i++){
			pChinese = NULL;
			pLastChinese = pLastInput->m_pTotalRes[i];
			vector<CChineseNode*> vecChineseBuff;
			pLastState = pLastChinese->m_pLeftFinal;
			while (pLastState != NULL) {
				pBigramTotal = pLastState->m_pChinese->m_pChild;
				nSize = pLastState->m_pChinese->m_nSize;
				for (int j = 0; j < nSize; j++) {
					if (j > 0 && strcmp(pBigramTotal[j]->m_pChinese, pBigramTotal[j - 1]->m_pChinese) != 0) {
						pChinese = NULL;
					}
					int pyTotal = pBigramTotal[j]->m_nID;
					int pyNum;
					char* cCurChinese = NULL;
					while (pChinese == NULL && pyTotal != 0) {
						pyNum = pyTotal % 1000 - 100;
						if (m_nTag[pyNum] == 1) {
							cCurChinese = MergeStr(pLastChinese->m_pTransChinese, pBigramTotal[j]->m_pChinese);
							for (int k = 0; k < vecChineseBuff.size(); k++) {
								if (strcmp(vecChineseBuff[k]->m_pTransChinese, cCurChinese) == 0) {
									pChinese = vecChineseBuff[k];
									break;
								}
							}
							if (pChinese == NULL) {
								pChinese = new CChineseNode();
								pChinese->m_pTransChinese = cCurChinese;
							}
							else {
								delete cCurChinese;
								cCurChinese = NULL;
							}
							break;
						}
						pyTotal = pyTotal / 1000;
					}
					if (pChinese != NULL) {
						//	cout << pBigramTotal[j]->m_pChinese << endl;
						if (pBigramTotal[j]->m_nState == 3) {
							pState = new CStateNode();
							pState->SetProb(pLastState->m_nProb + pBigramTotal[j]->m_nTransPorb);
							pState->SetSplit(pLastState->m_nSplit + 1);
							pBigramNode = m_pBigramRoot->GetWord(pBigramTotal[j]->m_pChinese);
							pState->m_pChinese = pBigramNode;
							pChinese->InsertFinal(pState);
							chinese.insert(pBigramTotal[j]->m_pChinese);
						}
						else if (pBigramTotal[j]->m_nState == 2) {
							pState = new CStateNode(pBigramTotal[j]);
							pState->SetProb(pLastState->m_nProb);
							pState->SetSplit(pLastState->m_nSplit);
							pChinese->InsertMid(pState);
						}
						if (cCurChinese != NULL) {
							if (pChinese->m_pLeftFinal != NULL || pChinese->m_pLeftMid != NULL) {
								vecChineseBuff.push_back(pChinese);
							}
							else {
								delete pChinese;
								pChinese = NULL;
							}
						}
					}
				}
				pLastState = pLastState->m_pNext;
			}
			pChinese = NULL;
			pLastState = pLastChinese->m_pLeftMid;
			while (pLastState != NULL) {
				pBigramTotal = pLastState->m_pChinese->m_pChild;
				nSize = pLastState->m_pChinese->m_nSize;
				for (int j = 0; j < nSize; j++) {
					if (j > 0 && strcmp(pBigramTotal[j]->m_pChinese, pBigramTotal[j - 1]->m_pChinese) != 0) {
						pChinese = NULL;
					}
					int pyTotal = pBigramTotal[j]->m_nID;
					int pyNum;
					char* cCurChinese = NULL;
					while (pChinese == NULL && pyTotal != 0) {
						pyNum = pyTotal % 1000 - 100;
						if (m_nTag[pyNum] == 1) {
							cCurChinese = MergeStr(pLastChinese->m_pTransChinese, pBigramTotal[j]->m_pChinese);
							for (int k = 0; k < vecChineseBuff.size(); k++) {
								if (strcmp(vecChineseBuff[k]->m_pTransChinese, cCurChinese) == 0) {
									pChinese = vecChineseBuff[k];
									break;
								}
							}
							if (pChinese == NULL) {
								pChinese = new CChineseNode();
								pChinese->m_pTransChinese = cCurChinese;
							}
							else {
								delete cCurChinese;
								cCurChinese = NULL;
							}
							break;
						}
						pyTotal = pyTotal / 1000;
					}
					if (pChinese != NULL) {
						//	cout << pBigramTotal[j]->m_pChinese << pBigramTotal[j]->m_nState << endl;
						if (pBigramTotal[j]->m_nState == 1) {
							pState = new CStateNode(pBigramTotal[j]);
							pState->SetProb(pLastState->m_nProb + pBigramTotal[j]->m_nTransPorb);
							pState->SetSplit(pLastState->m_nSplit + 1);
							pChinese->InsertFinal(pState);
						}
						else if (pBigramTotal[j]->m_nState == 3) {
							pState = new CStateNode();
							pState->SetProb(pLastState->m_nProb + pBigramTotal[j]->m_nTransPorb);
							pState->SetSplit(pLastState->m_nSplit + 1);
							char* totalWord = pBigramTotal[j]->GetTotalWord();
							pBigramNode = m_pBigramRoot->GetWord(totalWord);
							delete[] totalWord;
							pState->m_pChinese = pBigramNode;
							pChinese->InsertFinal(pState);
						}
						else {
							pState = new CStateNode(pBigramTotal[j]);
							pState->SetProb(pLastState->m_nProb);
							pState->SetSplit(pLastState->m_nSplit);
							pChinese->InsertMid(pState);
						}
						if (cCurChinese != NULL) {
							if (pChinese->m_pLeftFinal != NULL || pChinese->m_pLeftMid != NULL) {
								vecChineseBuff.push_back(pChinese);
							}
							else {
								delete pChinese;
								pChinese = NULL;
							}
						}
					}
				}
				pLastState = pLastState->m_pNext;
			}
			for (int k = 0; k < vecChineseBuff.size(); k++) {
				pCurString->InsertChinese(vecChineseBuff[k]);
			}
			vecChineseBuff.clear();
		}
		InputJoint(pCurString, pLastInput, chinese);
	}



	void InputStepNew(string str) {
		m_pSegment->AddStep(str);
		map<string, vector<char*>> mapSegmentRes = m_pSegment->GetNewResult();
		//	cout << "SegmentSize" << mapSegmentRes.size() << endl;
		//		m_pSegment->Log();

		CInputString* pNewInput = new CInputString();
		CInputString* pLast;
		char* pLastInputNum = NULL;
		if (m_pHead->m_pNext != NULL) {
			pLastInputNum = m_pHead->m_pNext->m_pInputStr;
		}
		char* cstr = new char[2];
		cstr[0] = str[0];
		cstr[1] = '\0';
		pNewInput->m_pInputStr = MergeStr(pLastInputNum, cstr);
		for (auto ite = mapSegmentRes.begin(); ite != mapSegmentRes.end(); ite++) {
			for (int i = 0; i < ite->second.size(); i++) {
				string strInput(ite->second[i]);
				vector<string> vecPinyin = m_pSegment->getpy(strInput);
				SetTag(vecPinyin);
				delete[] ite->second[i];
			}
			char* cLastInput;
			if (ite->first.size() == 0) {
				cLastInput = NULL;
			}
			else {
				cLastInput = new char[ite->first.size() + 1];
				strcpy_s(cLastInput, ite->first.length() + 1, ite->first.c_str());
			}
			InputStringNew(pNewInput, cLastInput);
			delete[] cLastInput;
			ClearTag();
		}
		mapSegmentRes.clear();
		pNewInput->m_pNext = m_pHead->m_pNext;
		m_pHead->m_pNext = pNewInput;
		pNewInput->SortTrans();
		pNewInput->Filter();

	}


	void DeleteStep() {
		m_pSegment->DeleteStep();
		if (m_pHead->m_pNext == NULL) {
			return;
		}
		CInputString* pTemp = m_pHead->m_pNext;
		m_pHead->m_pNext = pTemp->m_pNext;
		delete pTemp;
	}

	void DeleteStepOld() {
		m_pSegment->DeleteStep();
		if (m_pHead->m_pNext == NULL) {
			return;
		}
		CInputNumber* pCurStep = m_pStep;
		for (int i = 0; i < pCurStep->m_nSize; i++) {
			if (pCurStep->m_pTotalRes[i] != NULL) {
				DeleteInputString(pCurStep->m_pTotalRes[i]);
			}
		}
		m_pStep = m_pStep->m_pNext;
		delete pCurStep;
	}

	void ClearAll() {
		m_pSegment->ClearAll();
		CInputString* cur = m_pHead->m_pNext;
		CInputString* temp;
		while (cur != NULL) {
			temp = cur;
			cur = cur->m_pNext;
			delete temp;
		}
		m_pHead->m_pNext = NULL;
		CInputNumber* curNumber = m_pStep;
		CInputNumber* tempNumer;
		while (curNumber != NULL) {
			tempNumer = curNumber;
			curNumber = curNumber->m_pNext;
			delete tempNumer;
		}
		m_pStep = NULL;
	}

	void PrintLog() {
		ofstream fout("logNew.txt", ofstream::app);
		if (m_pHead->m_pNext == NULL) {
			cout << "nothing input" << endl;
			return;
		}
		CInputString* pCurStep = m_pHead->m_pNext;
		vector<char*> vecResult;
		CStateNode* pCurState;
		char* cWord;
		for (int i = 0; i < pCurStep->m_nTranSize; i++) {
			fout << "--------------------------" << endl;
			fout << pCurStep->m_pTotalRes[i]->m_pTransChinese << "  " << pCurStep->m_pTotalRes[i]->m_nMinSplit << endl;
			fout << "Final State:" << endl;
			pCurState = pCurStep->m_pTotalRes[i]->m_pLeftFinal;
			while (pCurState != NULL) {
				cWord = pCurState->m_pChinese->GetTotalWord();
				fout << cWord << "    " << pCurState->m_nProb << "    " << pCurState->m_nSplit << endl;
				//fout << pCurState->m_pChinese->m_pChinese << endl;
				delete[] cWord;
				pCurState = pCurState->m_pNext;
			}
			fout << "Mid State:" << endl;
			pCurState = pCurStep->m_pTotalRes[i]->m_pLeftMid;
			while (pCurState != NULL) {
				cWord = pCurState->m_pChinese->GetTotalWord();
				fout << cWord << "    " << pCurState->m_nProb << "    " << pCurState->m_nSplit << endl;
				// fout << pCurState->m_pChinese->m_pChinese << endl;
				pCurState = pCurState->m_pNext;
				delete[] cWord;
			}
		}
		fout.close();
	}

	vector<char*> GetTransRes() {
		vector<char*> vecResult;
		if (m_pHead->m_pNext == NULL) {
			cout << "nothing input" << endl;
			return vecResult;
		}
		CInputString* pCurStep = m_pHead->m_pNext;
		//		cout << pCurStep->m_nTranSize << endl;
		int flag = 0;
		for (int i = 0; i < pCurStep->m_nTranSize; i++) {
			if (pCurStep->m_pTotalRes[i]->m_pLeftFinal != NULL) {
				if (pCurStep->m_pTotalRes[i]->m_nMinSplit == 1) {
					vecResult.push_back(pCurStep->m_pTotalRes[i]->m_pTransChinese);
					flag = 1;
				}
				else {
					if (flag == 0) {
						vecResult.push_back(pCurStep->m_pTotalRes[i]->m_pTransChinese);
						flag = 1;
					}
					else {
						break;
					}
				}
			}
		}
		return vecResult;
	}


	bool IsEmpty() {
		if (m_pHead->m_pNext == NULL) {
			return 1;
		}
		else {
			return 0;
		}
	}

};