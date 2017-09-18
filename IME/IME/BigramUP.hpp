#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <queue>
#include "FunctionGroup.hpp"
#include "Entry.hpp"

using namespace std;

class CBigramNode {
public:
	char* m_pChinese;
	unsigned short m_nState;	//0: 第一个词条的非终止状态，1：第一个词条的终止状态，2：第二个词条的非终止状态，3：第二个词条的终止状态
	int m_nCount;
	int m_nTransPorb;
	CBigramNode* m_pLChild;
	CBigramNode* m_pRChild;
	CBigramNode* m_pBrother;
	CBigramNode* m_pParent;
	
	CBigramNode() {
		m_pChinese = NULL;
		m_nState = 0;
		m_nCount = 0;
		m_nTransPorb = 0;
		m_pLChild = NULL;
		m_pRChild = NULL;
		m_pParent = NULL;
		m_pBrother = NULL;
	}

	CBigramNode(unsigned short s, char* c) {
		m_pChinese = c;
		m_nState = s;
		m_nCount = 0;
		m_nTransPorb = 0;
		m_pLChild = NULL;
		m_pRChild = NULL;
		m_pParent = NULL;
		m_pBrother = NULL;
	}

	~CBigramNode() {
		delete m_pLChild;
		delete m_pChinese;
		delete m_pRChild;
		delete m_pParent;
		delete m_pBrother;
	}

	void AddCount() {
		m_nCount++;
	}

	CBigramNode* FindNode(char* cstr) {
		CBigramNode* curNode = m_pLChild;
		while (curNode != NULL) {
			if (strcmp(curNode->m_pChinese, cstr) == 0) {
				if (curNode->m_nState == 1 || curNode->m_nState == 3) {
					return curNode;
				}
			}
			curNode = curNode->m_pBrother;
		}
		return NULL;
	}
	CBigramNode* FindMidState(char* cstr) {
		CBigramNode* curNode = m_pLChild;
		while (curNode != NULL) {
			if (strcmp(curNode->m_pChinese, cstr) == 0) {
				if (curNode->m_nState == 0 || curNode->m_nState == 2) {
					return curNode;
				}
			}
			curNode = curNode->m_pBrother;
		}
		return NULL;
	}

	void InsertChild(CBigramNode* b) {
		if (m_pLChild == NULL) {
			m_pLChild = b;
			m_pRChild = b;
		}
		else {
			m_pRChild->m_pBrother = b;
			m_pRChild = b;
		}
	}

	void PrintLog() {
		ofstream fout("log.txt");
		CBigramNode* cur = m_pLChild;
		stack<CBigramNode*> staBigNode;
		CBigramNode* pParent;
		if (m_nState == 1 || m_nState == 3) {
			cout << 'a' << endl;
			if (m_nState == 3) {
				fout << "   ";
			}
			cout << 'a';
			pParent = m_pParent;
			while (pParent != NULL && pParent->m_nState != 1) {
				staBigNode.push(pParent);
				pParent = pParent->m_pParent;
			}
			while (!staBigNode.empty()) {
				fout << staBigNode.top()->m_pChinese;
				staBigNode.pop();
			}
			fout << m_pChinese << "  |  the number of word " << m_nCount << "  |  the prob of word " << m_nTransPorb << endl;
		}
		else {
			while (cur != NULL) {
				cur->PrintLog();
				cur = cur->m_pBrother;
			}
		}
		fout.close();
	}

	void SetProb() {

	}

	char* GetTotalWord() {
		CBigramNode* pParent = m_pParent;
		stack<char*> staChar;
		staChar.push(m_pChinese);
		while (pParent != NULL && pParent->m_nState != 1) {
			if (pParent->m_pChinese != NULL) {
				staChar.push(pParent->m_pChinese);
			}
			pParent = pParent->m_pParent;
		}
		char* result;
		char* ptemp = new char[1];
		ptemp[0] = '\0';
		while (!staChar.empty()) {
			result = MergeStr(ptemp, staChar.top());
			delete[] ptemp;
			ptemp = result;
		}
		return result;
	}
};

class CBigramTree {
public:
	int m_nTotal;
	CBigramNode* m_pRoot;
	CEntryTree* m_pEntryTree;
	
	CBigramTree() {
		m_nTotal = 0;
		m_pRoot = new CBigramNode();
		m_pEntryTree = new CEntryTree();
		CreateTree();
		m_pRoot->PrintLog();
	}

	void InitInsert(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr) - 3; i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidState(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			}
			else {
				tempNode = new CBigramNode(0, ctemp);
				cur->InsertChild(tempNode);
				tempNode->m_pParent = cur;
				cur = tempNode;
			}
		}
		ctemp = GetSubStr(cstr, strlen(cstr) - 3, 3);
		tempNode = cur->FindNode(ctemp);
		if (tempNode != NULL) {
			delete[] ctemp;
			return;
		}
		tempNode = new CBigramNode(1, ctemp);
		cur->InsertChild(tempNode);
		tempNode->m_pParent = cur;
	}

	void InsertWord(CBigramNode* cur, char* cstr) {
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr) - 3; i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidState(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			}
			else {
				tempNode = new CBigramNode(2, ctemp);
				cur->InsertChild(tempNode);
				tempNode->m_pParent = cur;
				cur = tempNode;
			}
		}
		ctemp = GetSubStr(cstr, strlen(cstr) - 3, 3);
		tempNode = cur->FindNode(ctemp);
		if (tempNode == NULL) {
			tempNode = new CBigramNode(3, ctemp);
			cur->InsertChild(tempNode);
			tempNode->m_pParent = cur;
		}
		else {
			delete[] ctemp;
		}
		tempNode->AddCount();
		
	}

	bool IsWord(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr) - 3; i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidState(ctemp);
			if (tempNode == NULL) {
				delete[] ctemp;
				return 0;
			}
			else {
				cur = tempNode;
			}
			delete[] ctemp;
		}
		ctemp = GetSubStr(cstr, strlen(cstr) - 3, 3);
		tempNode = cur->FindNode(ctemp);
		if (tempNode == NULL) {
			delete[] ctemp;
		}
		else {
			delete[] ctemp;
			return 1;
		}
	}

	void InsertBigram(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr); i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindNode(ctemp);
			if (tempNode != NULL) {
				if (i + 3 == strlen(cstr)) {
					tempNode->AddCount();
					m_nTotal++;
				}
				else{
					char* cGramWord = GetSubStr(cstr, i + 3, strlen(cstr) - i - 3);
					if (IsWord(cGramWord) == 1) {
						InsertWord(tempNode, cGramWord);
					}
					delete[] cGramWord;
				}
			}
			tempNode = cur->FindMidState(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
			}
			else {
				break;
			}
		}
	}

	void CreateTree() {
		string str;
		char* cstr;

		fstream fin("chsource.txt");
		if (!fin) {
			cout << "open file error" << endl;
			exit(1);
		}
		int index = 0;
		int num = 0;
		while (getline(fin, str)) {
			if (index % 3 == 1) {
				cstr = new char[str.length() + 1];
				strcpy_s(cstr, str.length() + 1, str.c_str());
				InitInsert(cstr);
				cout << UTF8ToGBK(cstr) << endl;
				delete[] cstr;
				num++;
			}
			index++;
			if (num > 10) {
				break;
			}
		}
		fin.close();

		fin.open("word.txt");
		if (!fin) {
			cout << "open file error" << endl;
			exit(1);
		}
		
		num = 0;
		while (getline(fin, str)) {
			cstr = new char[str.length() + 1];
			strcpy_s(cstr, str.length() + 1, str.c_str());
			InitInsert(cstr);
			cout << UTF8ToGBK(cstr) << endl;
			delete[] cstr;
			if (num > 10) {
				break;
			}
			num++;
		}
		fin.close();
		
		fin.close();
		
		/*fin.open("result.txt");
		if (!fin) {
			cout << "open file error" << endl;
			exit(1);
		}
		while (getline(fin, str)) {
			for (int i = 0; i < str.length(); i = i + 3) {
				for (int j = i; j < str.length(); j = j + 3) {
					cstr = new char[str.substr(i, j - i + 3).length() + 1];
					strcpy_s(cstr, str.substr(i, j - i + 3).length() + 1, str.substr(i, j - i + 3).c_str());
					InsertBigram(cstr);
					delete[] cstr;
				}
			}
		}
		fin.close();*/
	}

	bool FindWord(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr) - 3; i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidState(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			}
			else {
				return 0;
			}
		}
		ctemp = GetSubStr(cstr, strlen(cstr) - 3, 3);
		tempNode = cur->FindNode(ctemp);
		if (tempNode != NULL) {
			delete[] ctemp;
			return 1;
		}
		return 0;
	}

	CBigramNode* GetWord(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr) - 3; i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidState(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			}
			else {
				return NULL;
			}
		}
		ctemp = GetSubStr(cstr, strlen(cstr) - 3, 3);
		tempNode = cur->FindNode(ctemp);
		if (tempNode != NULL) {
			delete[] ctemp;
			return tempNode;
		}
		return NULL;
	}

	CBigramNode* GetMidState(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr); i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidState(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			}
			else {
				return NULL;
			}
		}
		return tempNode;
	}

	void SetProb() {
		CBigramNode* cur = m_pRoot;
		
	}
};



