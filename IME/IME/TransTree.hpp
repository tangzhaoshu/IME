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

using namespace std;

class CStateNode {
public:
	CBigramNode* m_pChinese;
	CStateNode* m_pNext;
	CStateNode* m_pParent;
	int m_nProb;

	CStateNode() {
		m_pChinese = NULL;
		m_pNext = NULL;
		m_pParent = NULL;
		m_nProb = 0;
	}

	CStateNode(CBigramNode* p) {
		m_pChinese = p;
	}

	void SetProb(int p) {
		m_nProb = p;
	}

	void SetNext(CStateNode* p) {
		m_pNext = p;
	}

	void SetChinese(CBigramNode* p) {
		m_pChinese = p;
	}

	~CStateNode() {
		m_pChinese = NULL;
		m_pNext = NULL;
		m_pParent = NULL;
		delete m_pChinese;
		delete m_pNext;
		delete m_pParent;
	}
};

class CChineseNode {
public:
	int m_nMinProb;
	char* m_pTransChinese;
	CStateNode* m_pLeftFinal;
	CStateNode* m_pLeftMid;
	CChineseNode* m_pNext;

	CChineseNode() {
		m_nMinProb = 0;
		m_pNext = NULL;
		m_pTransChinese = NULL;
		m_pLeftFinal = NULL;
		m_pLeftMid = NULL;
	}

	~CChineseNode() {
		m_pTransChinese = NULL;
		m_pNext = NULL;
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
		delete m_pTransChinese;
		delete m_pNext;
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
		}
		else{
			cur->m_pParent = cur->m_pNext;
		}
		delete cur;
		cur = m_pLeftFinal;
		if (cur = NULL) {
			m_nMinProb = 0;
		}
		int min = cur->m_nProb;
		while (cur != NULL) {
			if (min < cur->m_nProb) {
				min = cur->m_nProb;
			}
			cur = cur->m_pNext;
		}
		m_nMinProb = min;
	}

	void SetTrans(char* c) {
		m_pTransChinese = c;
	}

	void InsertFinal(CStateNode* c) {
		if (m_pLeftFinal = NULL) {
			m_pLeftFinal = c;
			m_nMinProb = c->m_nProb;
		}
		CStateNode* cur = m_pLeftFinal;
		if (cur->m_pChinese == c->m_pChinese) {
			if (cur->m_nProb < c->m_nProb) {
				delete c;
				return;
			}
			else {
				m_pLeftFinal = c;
				c->m_pNext = cur->m_pNext;
				cur->m_pNext->m_pParent = c;
				delete cur;
				return;
			}
		}
		while (cur->m_pNext != NULL) {
			if (cur->m_pNext->m_pChinese == c->m_pChinese) {
				if (cur->m_pNext->m_nProb < c->m_nProb) {
					delete c;
					return;
				}
				else {
					cur->m_pNext->m_pParent->m_pNext = c;
					c->m_pNext = cur->m_pNext->m_pNext;
					cur->m_pNext->m_pNext->m_pParent = c;
					delete cur->m_pNext;
					if (m_nMinProb > c->m_nProb) {
						m_nMinProb = c->m_nProb;
					}
					return;
				}
			}
			cur = cur->m_pNext;
		}
		cur->m_pNext = c;
		c->m_pParent = cur;
		if (m_nMinProb > c->m_nProb) {
			m_nMinProb = c->m_nProb;
		}
	}

	void InsertMid(CStateNode* c) {
		CStateNode* cur = m_pLeftMid;
		if (cur == NULL) {
			m_pLeftMid = c;
			return;
		}
		while (cur->m_pNext != NULL) {
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

};

class CInputString {
public:
	char* m_pInputStr;
	CChineseNode* m_pLeftRes;
	CInputString* m_pNext;

	CInputString() {
		m_pInputStr = NULL;
		m_pLeftRes = NULL;
		m_pNext = NULL;
	}

	~CInputString() {
		delete m_pInputStr;
		CChineseNode* cur = m_pLeftRes;
		CChineseNode* temp;
		while (cur != NULL) {
			temp = cur;
			cur = cur->m_pNext;
			delete temp;
		}
		m_pNext = NULL;
		delete m_pNext;
	}

	void DeleteChinese(CChineseNode* p) {
		if (m_pLeftRes == NULL) {
			cout << "Node not exist" << endl;
			return;
		}
		CChineseNode* cur = m_pLeftRes;
		if (cur == p) {
			m_pLeftRes = cur->m_pNext;
			delete cur;
			return;
		}
		while (cur->m_pNext != NULL) {
			if (cur->m_pNext == p) {
				cur->m_pNext = cur->m_pNext->m_pNext;
				delete p;
				return;
			}
			cur = cur->m_pNext;
		}
	}

	void InsertChinese(CChineseNode* p) {
		if (m_pLeftRes == NULL) {
			m_pLeftRes = p;
			return;
		}
		CChineseNode* curCh;
		CStateNode* curState = p->m_pLeftFinal;
		CStateNode* tempState;
		while (curState != NULL) {
			curCh = m_pLeftRes;
			while (curCh != NULL) {
				tempState = curCh->m_pLeftFinal;
				while (tempState != NULL) {
					if (tempState->m_pChinese == curState->m_pChinese) {
						break;
					}
					tempState = tempState->m_pNext;
				}
				if (tempState != NULL) {
					if (tempState->m_nProb > curState->m_nProb) {
						curCh->DeleteState(tempState);
						if (curCh->m_pLeftFinal == NULL && curCh->m_pLeftMid == NULL) {
							DeleteChinese(curCh);
						}
						break;
					}
					else {
						tempState = curState;
						curState = curState->m_pNext;
						p->DeleteState(tempState);
						break;
					}
				}
				curCh = curCh->m_pNext;
			}
			if (curCh == NULL) {
				curState = curState->m_pNext;
			}
		}
		if (p->m_pLeftFinal != NULL || p->m_pLeftMid != NULL) {
			if (m_pLeftRes == NULL) {
				m_pLeftRes = p;
			}
			else {
				curCh = m_pLeftRes;
				while (curCh->m_pNext != NULL) {
					curCh = curCh->m_pNext;
				}
				curCh->m_pNext = p;
			}
		}
	}
	

};

class CStep {
public:
	CInputString* m_pHead;
	CBigramTree* m_pBigramRoot;

	CStep() {
		m_pHead = new CInputString();
		m_pBigramRoot = new CBigramTree();
	}

	CInputString* FindHistory(char* c) {
		CInputString* cur = m_pHead;
		while (cur->m_pNext != NULL) {
			if (strcmp(cur->m_pInputStr, c) == 0) {
				return cur->m_pNext;
			}
			cur = cur->m_pNext;
		}
		return NULL;
	}

	void InputString(char* history, char* input, vector<char*> chinese) {
		CBigramNode* pBigramNode = NULL;
		CInputString* pLastInput = NULL;
		CStateNode* pState = NULL;
		CChineseNode* pChinese = NULL;
		CInputString* pCurString = new CInputString();
		pCurString->m_pInputStr = MergeStr(history, input);

		if (history != NULL) {
			pLastInput = FindHistory(history);
		}
		if (pLastInput == NULL) {
			for (int i = 0; i < chinese.size(); i++) {
				pChinese = new CChineseNode();
				pChinese->m_pTransChinese = chinese[i];
				pBigramNode = m_pBigramRoot->GetWord(chinese[i]);
				if (pBigramNode != NULL) {
					pState = new CStateNode(pBigramNode);
					pState->SetProb(pBigramNode->m_nTransPorb);
					pChinese->InsertFinal(pState);
				}
				pBigramNode = m_pBigramRoot->GetMidState(chinese[i]);
				if (pBigramNode != NULL) {
					pState = new CStateNode(pBigramNode);
					pChinese->InsertMid(pState);
				}
				if (pChinese->m_pLeftFinal != NULL || pChinese->m_pLeftMid != NULL) {
					pCurString->InsertChinese(pChinese);
				}
				else {
					delete pChinese;
				}
			}
			pCurString->m_pNext = m_pHead->m_pNext;
			m_pHead = pCurString;
			return;
		}

		CChineseNode* pLastChinese = NULL;
		CStateNode* pLastState = NULL;
		for (int i = 0; i < chinese.size(); i++) {
			pLastChinese = pLastInput->m_pLeftRes;
			while (pLastChinese != NULL) {
				pChinese = new CChineseNode();
				pChinese->m_pTransChinese = MergeStr(pLastChinese->m_pTransChinese, chinese[i]);

				pLastState = pLastChinese->m_pLeftFinal;
				while (pLastState != NULL) {
					pBigramNode = pLastState->m_pChinese->FindNode(chinese[i]);
					if (pBigramNode != NULL) {
						pState = new CStateNode();
						pState->SetProb(pLastState->m_nProb + pBigramNode->m_nTransPorb);
						pBigramNode = m_pBigramRoot->GetWord(chinese[i]);
						pState->m_pChinese = pBigramNode;
						pChinese->InsertFinal(pState);
					}
					else{
						pBigramNode = m_pBigramRoot->GetWord(chinese[i]);
						pState = new CStateNode(pBigramNode);
						pState->SetProb(pLastState->m_nProb + pBigramNode->m_nTransPorb);
						pChinese->InsertFinal(pState);
					}
					pBigramNode = pLastState->m_pChinese->FindMidState(chinese[i]);
					if (pBigramNode != NULL) {
						pState = new CStateNode(pBigramNode);
						pState->SetProb(pLastState->m_nProb);
						pChinese->InsertMid(pState);
					}
					pLastState = pLastState->m_pNext;
				}
				pLastState = pLastChinese->m_pLeftMid;
				while (pLastState != NULL) {
					pBigramNode = pLastState->m_pChinese->FindNode(chinese[i]);
					if (pBigramNode != NULL) {
						if (pBigramNode->m_nState == 1) {
							pState = new CStateNode(pBigramNode);
							pState->SetProb(pLastState->m_nProb + pBigramNode->m_nTransPorb);
							pChinese->InsertFinal(pState);
						}
						else if (pBigramNode->m_nState == 3) {
							pState = new CStateNode();
							pState->SetProb(pLastState->m_nProb + pBigramNode->m_nTransPorb);
							char* totalWord = pBigramNode->GetTotalWord();
							pBigramNode = m_pBigramRoot->GetWord(totalWord);
							delete[] totalWord;
							pState->m_pChinese = pBigramNode;
							pChinese->InsertFinal(pState);
						}
					}
					pBigramNode = pLastState->m_pChinese->FindMidState(chinese[i]);
					if (pBigramNode != NULL) {
						pState = new CStateNode(pBigramNode);
						pState->SetProb(pLastState->m_nProb);
						pChinese->InsertFinal(pState);
					}
					pLastState = pLastState->m_pNext;
				}
				if (pChinese->m_pLeftFinal != NULL || pChinese->m_pLeftMid != NULL) {
					pCurString->InsertChinese(pChinese);
				}
				else{
					delete pChinese;
				}
				pLastChinese = pLastChinese->m_pNext;
			}
		}
		pCurString->m_pNext = m_pHead->m_pNext;
		m_pHead = pCurString;
	}

	void Inputstring(char* cstr, int* pos, int size) {
		CInputString* pNewInput = new CInputString();
		CInputString* pLast;
		pNewInput->m_pInputStr = cstr;
		for (int i = 0; i < size; i++) {
			char* hist = GetSubStr(cstr, 0, strlen(cstr) - pos[i]);
			char* input = GetSubStr(cstr, strlen(cstr) - pos[i], pos[i]);
			
			
		}
		pNewInput->InputString()
	}

	
};