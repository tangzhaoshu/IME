#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include "Entry.hpp"

using namespace std;

class CBigramNode {
public:
	char* m_pChinese;
	int m_nState;	//0: 第一个词条的非终止状态，1：第一个词条的终止状态，2：第二个词条的非终止状态，3：第二个词条的终止状态
	int m_nCount;
	int m_nTransPorb;
	CBigramNode* m_pChild;
	CBigramNode* m_pBrother;
	
	CBigramNode() {
		m_pChinese = NULL;
		m_nState = 0;
		m_nCount = 0;
		m_nTransPorb = 0;
		m_pChild = NULL;
		m_pBrother = NULL;
	}

	~CBigramNode() {
		delete m_pChild;
		delete m_pChild;
		delete m_pBrother;
	}

	CBigramNode* FindNode(char* cstr) {
		CBigramNode* curNode = m_pChild;
		while (curNode != NULL && strcmp(curNode->m_pChinese, cstr) != 0) {
			curNode = curNode->m_pBrother;
		}
		return curNode;
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
	}

	void Insert(char* cstr) {

	}
};



