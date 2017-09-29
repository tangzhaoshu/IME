#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <string.h>
#include <queue>
#include <windows.h>
#include "FunctionGroup.cpp"

using namespace std;

int sTOTALSIZE = 0;
int sTOTALNODE = 0;
class CBigramNode {
public:
	char* m_pChinese;
	CBigramNode* m_pParent;
	CBigramNode** m_pChild;
	int m_nID;
	unsigned short m_nTransPorb;
	unsigned short m_nState;	//0: 第一个词条的非终止状态，1：第一个词条的终止状态，2：第二个词条的非终止状态，3：第二个词条的终止状态
	//int m_nCount;
	unsigned short m_nSize;
	unsigned short m_nCapacity;


	CBigramNode() {
		sTOTALNODE ++;
		m_pChinese = NULL;
		m_nState = 0;
		m_nTransPorb = 0;
		m_nID = 0;
		m_pParent = NULL;
		m_pChild = NULL;
		m_nSize = 0;
		m_nCapacity = 0;
	}

	CBigramNode(unsigned short s, char* c, int i) {
		sTOTALNODE ++;
		m_pChinese = c;
		m_nState = s;
		m_nTransPorb = 0;
		m_pParent = NULL;
		m_pChild = NULL;
		m_nSize = 0;
		m_nCapacity = 0;
		m_nID = i;
	}

	CBigramNode(const CBigramNode &c) {
		sTOTALNODE ++;
		m_pChinese = c.m_pChinese;
		m_pParent = c.m_pParent;
		m_pChild = c.m_pChild;
		m_nTransPorb = c.m_nTransPorb;
		m_nState = c.m_nState;
		m_nID = c.m_nID;
		m_nSize = c.m_nSize;
		m_nCapacity = c.m_nCapacity;
	}

	void SetChinese(unsigned short s, char* c) {
		m_pChinese = c;
		m_nState = s;
	}

	void SetSize(int size) {
		if (size == 0) {
			return;
		}
		m_nCapacity = size;
		m_pChild = new CBigramNode*[m_nCapacity];
	}

	~CBigramNode() {
		delete m_pChinese;
		m_pParent = NULL;
		delete m_pParent;
		for (int i = 0; i < m_nSize; i++) {
			delete m_pChild[i];
		}
		delete[] m_pChild;
	}

	void Clear() {
		m_pChinese = NULL;
		m_nState = 0;
		m_nTransPorb = 0;
		m_pParent = NULL;
		m_pChild = NULL;
		m_nSize = 0;
		m_nCapacity = 0;
	}

	void SumSize() {
		if (m_pChinese != NULL) {
			sTOTALSIZE += strlen(m_pChinese) + 1;
		}
		sTOTALSIZE += m_nCapacity * sizeof(m_pParent);
		for (int i = 0; i < m_nSize; i ++) {
			sTOTALSIZE += sizeof(*m_pChild[i]);
			m_pChild[i]->SumSize();
		}
	}

	//单步搜索词条终止节点
	CBigramNode* FindNodeObO(char* cstr) {
		if (m_nSize == 0) {
			return NULL;
		}
		CBigramNode** arrayNode = m_pChild;
		//cout << 9 << endl;
		for (int i = 0; i < m_nSize; i++) {
			if (strcmp(arrayNode[i]->m_pChinese, cstr) == 0) {
				if (arrayNode[i]->m_nState == 1 || arrayNode[i]->m_nState == 3) {
					return arrayNode[i];
				}
			}
		}
		return NULL;
	}

	//二分搜索寻找词条终止节点
	CBigramNode* FindNode(char* cstr) {
		CBigramNode** arrayNode = m_pChild;
		if (arrayNode == NULL) {
			return NULL;
		}
		int mid = m_nSize / 2;
		int left = 0;
		int right = m_nSize - 1;
		while (left <= right) {
			if (strcmp(arrayNode[mid]->m_pChinese, cstr) == 0) {
				if (arrayNode[mid]->m_nState == 1 || arrayNode[mid]->m_nState == 3) {
					return arrayNode[mid];
				}
				else {
					mid = mid + 1;
					if ((mid <= right) && strcmp(arrayNode[mid]->m_pChinese, cstr) == 0) {
						return arrayNode[mid];
					}
					mid = mid - 2;
					if ((mid >= left) && strcmp(arrayNode[mid]->m_pChinese, cstr) == 0) {
						return arrayNode[mid];
					}
					return NULL;
				}
			}
			else if (strcmp(arrayNode[mid]->m_pChinese, cstr) < 0) {
				left = mid + 1;
			}
			else {
				right = mid - 1;
			}
			mid = (left + right) / 2;
		}
		return NULL;
	}

	//单步搜索词条中间节点
	CBigramNode* FindMidStateObO(char* cstr) {
		if (m_nSize == 0) {
			return NULL;
		}
		CBigramNode** arrayNode = m_pChild;
		for (int i = 0; i < m_nSize; i++) {
			if (strcmp(arrayNode[i]->m_pChinese, cstr) == 0) {
				if (arrayNode[i]->m_nState == 0 || arrayNode[i]->m_nState == 2) {
					return arrayNode[i];
				}
			}
		}
		return NULL;
	}

	//二分搜索词条中间节点
	CBigramNode* FindMidState(char* cstr) {
		CBigramNode** arrayNode = m_pChild;
		if (arrayNode == NULL) {
			return NULL;
		}
		int mid = m_nSize / 2;
		int left = 0;
		int right = m_nSize - 1;
		while (left <= right) {
			if (strcmp(arrayNode[mid]->m_pChinese, cstr) == 0) {
				if (arrayNode[mid]->m_nState == 0 || arrayNode[mid]->m_nState == 2) {
					return arrayNode[mid];
				}
				else {
					mid = mid + 1;
					if ((mid <= right) && strcmp(arrayNode[mid]->m_pChinese, cstr) == 0) {
						return arrayNode[mid];
					}
					mid = mid - 2;
					if ((mid >= left) && strcmp(arrayNode[mid]->m_pChinese, cstr) == 0) {
						return arrayNode[mid];
					}
					return NULL;
				}
			}
			else if (strcmp(arrayNode[mid]->m_pChinese, cstr) < 0) {
				left = mid + 1;
			}
			else {
				right = mid - 1;
			}
			mid = (left + right) / 2;
		}
		return NULL;
	}

	void InsertChild(CBigramNode* b) {
		if (m_nSize < m_nCapacity) {
			m_pChild[m_nSize] = b;
			m_nSize++;
			return;
		}
		if (m_nCapacity == 0) {
			m_nCapacity = 1;
		}
		else {
			m_nCapacity += m_nCapacity;
		}
		CBigramNode** arrayTemp = m_pChild;
		m_pChild = new CBigramNode*[m_nCapacity];
		for (int i = 0; i < m_nSize; i++) {
			m_pChild[i] = arrayTemp[i];
			arrayTemp[i] = NULL;
		}
		delete[] arrayTemp;
		m_pChild[m_nSize] = b;
		m_nSize++;
	}

	//输入词条及Bigram转移概率等资源信息
	void PrintSource() {
		ofstream fout ("BigramSource.txt", ofstream::app);
		stack<char*> staChar;
		CBigramNode* pParent;

		if (m_nState == 1 || m_nState == 3) {
			if (m_nState == 1) {
				fout << 1 << endl;
			}
			pParent = m_pParent;
			while (pParent != NULL && pParent->m_nState != 1) {
				if (pParent->m_pChinese != NULL) {
					staChar.push(pParent->m_pChinese);
				}
				pParent = pParent->m_pParent;
			}
			while (!staChar.empty()) {
				fout << staChar.top();
				staChar.pop();
			}
			fout << m_pChinese << " " << m_nTransPorb << endl;
		}
		for (int i = 0; i < m_nSize; i++) {
			m_pChild[i]->PrintSource();
		}
		fout.close();
	}

	void PrintAllSource() {
		ofstream fout ("BigramSourceNew.txt", ofstream::app);
		stack<CBigramNode*> staChar;
		CBigramNode* pParent;
		if (m_nState == 1 || m_nState == 3) {
			if (m_nState == 1) {
				fout << 1 << endl;
			}
			pParent = m_pParent;
			while (pParent != NULL && pParent->m_nState != 1) {
				if (pParent->m_pChinese != NULL) {
					staChar.push(pParent);
				}
				pParent = pParent->m_pParent;
			}
			while (!staChar.empty()) {
				fout << staChar.top()->m_pChinese << " ";
				fout << staChar.top()->m_nSize << " ";
				staChar.pop();
			}
			fout << m_pChinese << " " << m_nSize << " " << m_nTransPorb << endl;
		}
		for (int i = 0; i < m_nSize; i++) {
			m_pChild[i]->PrintAllSource();
		}
		fout.close();
	}

	void PrintLog() {
		ofstream fout("logGram.txt", ofstream::app);
		stack<char*> staChar;
		CBigramNode* pParent;
		//cout << m_pChinese << endl;

		if (m_nState == 1 || m_nState == 3) {
			if (m_nState == 3) {
				fout << "   ";
			} else {
				fout << "*";
			}
			pParent = m_pParent;
			while (pParent != NULL && pParent->m_nState != 1) {
				if (pParent->m_pChinese != NULL) {
					staChar.push(pParent->m_pChinese);
				}
				pParent = pParent->m_pParent;
			}
			while (!staChar.empty()) {
				fout << staChar.top();
				staChar.pop();
			}
			fout << m_pChinese << "  |  the prob of word " << m_nTransPorb << endl;
		}
		for (int i = 0; i < m_nSize; i++) {
			m_pChild[i]->PrintLog();
		}
		fout.close();
	}
/*
	void SetProb() {
		CBigramNode* cur;
		CBigramNode** child;
		stack<CBigramNode*> staNode;

		for (int i = 0; i < m_nSize; i++) {
			staNode.push(m_pChild[i]);
		}
		while (!staNode.empty()) {
			cur = staNode.top();
			staNode.pop();
			child = cur->m_pChild;
			if (cur->m_nState == 3) {
				cur->m_nTransPorb = log(static_cast<double>(cur->m_nCount) / static_cast<double>(m_nCount)) * (-1);
			}
			else {
				for (int i = 0; i < cur->m_nSize; i++) {
					staNode.push(child[i]);
				}
			}
		}
	}
	*/

	//根据当前节点获取整个词条字符串
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
		char* result = NULL;
		char* ptemp = new char[1];
		ptemp[0] = '\0';
		while (!staChar.empty()) {
			result = MergeStr(ptemp, staChar.top());
			delete[] ptemp;
			ptemp = result;
			staChar.pop();
		}
		return result;
	}

	int Partition(CBigramNode** Array, int left, int right) {
		int l = left;
		int r = right;
		CBigramNode* temp = Array[r];
		while (l != r) {
			while (r > l && strcmp(Array[l]->m_pChinese, temp->m_pChinese) <= 0) {
				l++;
			}
			if (l < r) {
				Array[r] = Array[l];
				r--;
			}
			while (r > l && strcmp(Array[r]->m_pChinese, temp->m_pChinese) >= 0) {
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

	void QuickSort(CBigramNode** Array, int left, int right) {
		if (right <= left) {
			return;
		}
		int pivot = (left + right) / 2;

		CBigramNode* temp;
		temp = Array[pivot];
		Array[pivot] = Array[right];
		Array[right] = temp;

		pivot = Partition(Array, left, right);
		QuickSort(Array, left, pivot - 1);
		QuickSort(Array, pivot + 1, right);
	}

	void SortTree() {
		if (m_nSize == 0) {
			return;
		}
		if (m_nSize < m_nCapacity) {
			cout << m_nCapacity << " " << m_nSize << endl;
			CBigramNode** arrayNode = m_pChild;
			m_pChild = new CBigramNode*[m_nSize];
			for (int i = 0; i < m_nSize; i++) {
				m_pChild[i] = arrayNode[i];
				arrayNode[i] = NULL;
			}
			delete[] arrayNode;
			m_nCapacity = m_nSize;
		}
		QuickSort(m_pChild, 0, m_nSize - 1);
		for (int i = 0; i < m_nSize; i++) {
			m_pChild[i]->SortTree();
		}
	}
};

int SUM_NODE = 0;
int SUM_WORD = 0;

class CBigramTree {
public:
	int m_nTotal;
	CBigramNode* m_pRoot;
	map<string, int> m_ChTable;

	CBigramTree() {
		m_nTotal = 0;
		m_pRoot = new CBigramNode();
		GetChineseTable();
		cout << sizeof(*m_pRoot) << endl;
		//CreateTree();
		Create();
	//	SumSize();
	//	SortTree();
		//SetProb();
	//	PrintLog();
	//	PrintAllSource();
	}
	~CBigramTree() {
		delete m_pRoot;
	}

	void SumSize() {
		sTOTALSIZE += sizeof(*m_pRoot);
		m_pRoot->SumSize();
	}
	void PrintAllSource() {
		ofstream fout ("BigramSourceNew.txt");
		fout << "BigramSource" << endl;
		fout.close();
		m_pRoot->PrintAllSource();
	}

	void PrintLog() {
		ofstream fout("logGram.txt");
		for (auto ite = m_ChTable.begin(); ite != m_ChTable.end(); ite ++) {
			fout << ite->first << "  " << ite->second << endl;
		}
		fout.close();
		m_pRoot->PrintLog();
	}

	CBigramNode* GetRoot() {
		return m_pRoot;
	}

	void GetChineseTable(){
		fstream fin("chsource.txt");
		if (!fin) {
			cout << "open file error" << endl;
			exit(1);
		}
		int index = 0;
		string str;
		string value;
		while (getline(fin, str)) {
			if (index == 0) {
				value = str;
				index++;
			}
			else if (index == 1) {
				auto ite = m_ChTable.find(str);
				if (ite != m_ChTable.end()) {
					m_ChTable[str] = m_ChTable[str] * 1000 + StringToInt(value);
				} else {
					m_ChTable[str] = StringToInt(value);
				}
				index++;
			}
			else if (index == 2) {
				index = 0;
			}
		}
		fin.close();
	}
/*
	void InitInsert(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr) - 3; i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidStateObO(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			}
			else {
				tempNode = new CBigramNode(0, ctemp, m_ChTable[ctemp]);
				SUM_NODE++;
				tempNode->m_pParent = cur;
				cur->InsertChild(tempNode);
				cur = tempNode;
			}
		}
		ctemp = GetSubStr(cstr, strlen(cstr) - 3, 3);
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode != NULL) {
			delete[] ctemp;
			return;
		}
		tempNode = new CBigramNode(1, ctemp, m_ChTable[ctemp]);
		SUM_NODE++;
		tempNode->m_pParent = cur;
		cur->InsertChild(tempNode);
	}

	void InsertWord(CBigramNode* cur, char* cstr) {
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr) - 3; i = i + 3) {

			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidStateObO(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			}
			else {
				tempNode = new CBigramNode(2, ctemp, m_ChTable[ctemp]);
				SUM_NODE++;
				tempNode->m_pParent = cur;
				cur->InsertChild(tempNode);
				cur = tempNode;
			}
		}
		ctemp = GetSubStr(cstr, strlen(cstr) - 3, 3);
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode == NULL) {
			tempNode = new CBigramNode(3, ctemp, m_ChTable[ctemp]);
			SUM_NODE++;
			tempNode->m_pParent = cur;
			cur->InsertChild(tempNode);
		}
		else {
			delete[] ctemp;
		}
		tempNode->AddCount();

	}
*/

	bool IsWord(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr) - 3; i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidStateObO(ctemp);
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
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode == NULL) {
			delete[] ctemp;
			return 0;
		}
		else {
			delete[] ctemp;
			return 1;
		}
	}

	/*
	void InsertBigram(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr); i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindNodeObO(ctemp);
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
			tempNode = cur->FindMidStateObO(ctemp);
			delete[] ctemp;
			if (tempNode != NULL) {
				cur = tempNode;
			}
			else {
				break;
			}
		}
	}*/

	void InsertFirstWord(char* cstr, int nProb) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr) - 3; i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidStateObO(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			} else {
				tempNode = new CBigramNode(0, ctemp, m_ChTable[ctemp]);
				tempNode->m_pParent = cur;
				cur->InsertChild(tempNode);
				cur = tempNode;
			}
		}
		ctemp = GetSubStr(cstr, strlen(cstr) - 3, 3);
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode != NULL) {
			delete[] ctemp;
			return;
		}
		tempNode = new CBigramNode(1, ctemp, m_ChTable[ctemp]);
		tempNode->m_nTransPorb = nProb;
		tempNode->m_pParent = cur;
		cur->InsertChild(tempNode);
	}

	void InsertFirstWordNew(vector<string> vecFirst, int nProb) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < vecFirst.size() - 2; i = i + 2) {
			ctemp = GetSubStr(vecFirst[i].c_str(), 0, 3);
			tempNode = cur->FindMidStateObO(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			} else {
				int size = StringToInt(vecFirst[i + 1]);
				tempNode = new CBigramNode(0, ctemp, m_ChTable[ctemp]);
				tempNode->SetSize(size);
				tempNode->m_pParent = cur;
				cur->InsertChild(tempNode);
				cur = tempNode;
			}
		}
		ctemp = GetSubStr(vecFirst[vecFirst.size() - 2].c_str(), 0, 3);
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode != NULL) {
			delete[] ctemp;
			return;
		}
		int size = StringToInt(vecFirst[vecFirst.size() - 1]);
		tempNode = new CBigramNode(1, ctemp, m_ChTable[ctemp]);
		tempNode->SetSize(size);
		tempNode->m_nTransPorb = nProb;
		tempNode->m_pParent = cur;
		cur->InsertChild(tempNode);
	}

	void InsertSecondWord(char* cFirst, char* cSecond, int nProb) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cFirst) - 3; i = i + 3) {
			ctemp = GetSubStr(cFirst, i, 3);
			tempNode = cur->FindMidStateObO(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			} else {
				cout << "insert error" << endl;
				return;
			}
		}
		ctemp = GetSubStr(cFirst, strlen(cFirst) - 3, 3);
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode != NULL) {
			cur = tempNode;
			delete[] ctemp;
		} else {
			cout << "insert error" << endl;
			return;
		}

		for (int i = 0; i < strlen(cSecond) - 3; i = i + 3) {
			ctemp = GetSubStr(cSecond, i, 3);
			tempNode = cur->FindMidStateObO(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			} else {
				tempNode = new CBigramNode(2, ctemp, m_ChTable[ctemp]);
				tempNode->m_pParent = cur;
				cur->InsertChild(tempNode);
				cur = tempNode;
			}
		}
		ctemp = GetSubStr(cSecond, strlen(cSecond) - 3, 3);
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode != NULL) {
		delete[] ctemp;
			return;
		}
		tempNode = new CBigramNode(3, ctemp, m_ChTable[ctemp]);
		tempNode->m_nTransPorb = nProb;
		tempNode->m_pParent = cur;
		cur->InsertChild(tempNode);

	}

	void InsertSecondWordNew(vector<string> vecFirst, vector<string> vecSecond, int nProb) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < vecFirst.size() - 2; i = i + 2) {
			ctemp = GetSubStr(vecFirst[i].c_str(), 0, 3);
			tempNode = cur->FindMidStateObO(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			} else {
				cout << "insert error" << endl;
				return;
			}
		}
		ctemp = GetSubStr(vecFirst[vecFirst.size() - 2].c_str(), 0, 3);
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode != NULL) {
			cur = tempNode;
			delete[] ctemp;
		} else {
			cout << "insert error" << endl;
			return;
		}

		for (int i = 0; i < vecSecond.size() - 2; i = i + 2) {
			ctemp = GetSubStr(vecSecond[i].c_str(), 0, 3);
			tempNode = cur->FindMidStateObO(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			} else {
				int size = StringToInt(vecSecond[i + 1]);
				tempNode = new CBigramNode(2, ctemp, m_ChTable[ctemp]);
				tempNode->SetSize(size);
				tempNode->m_pParent = cur;
				cur->InsertChild(tempNode);
				cur = tempNode;
			}
		}
		ctemp = GetSubStr(vecSecond[vecSecond.size() - 2].c_str(), 0, 3);
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode != NULL) {
		delete[] ctemp;
			return;
		}
		int size = StringToInt(vecSecond[vecSecond.size() - 1]);
		tempNode = new CBigramNode(3, ctemp, m_ChTable[ctemp]);
		tempNode->SetSize(size);
		tempNode->m_nTransPorb = nProb;
		tempNode->m_pParent = cur;
		cur->InsertChild(tempNode);

	}

	void Create() {
		string str;
		char* cFirst = NULL;
		char* cSecond = NULL;
		int nprob;
		fstream fin("BigramSource.txt");
		if (!fin) {
			cout << "open file error" << endl;
			exit(1);
		}
		vector<string> vecStr;
		getline(fin, str);
		cout << str << endl;
		while (getline(fin, str)) {
			if (str == "1") {
				delete[] cFirst;
				getline(fin, str);
				vecStr = SplitStr(str);
				if (vecStr.size() != 2) {
					cout << str << endl;
					cout << vecStr.size() << endl;
					cout << "read file error" << endl;
					return;
				}
				cFirst = new char[vecStr[0].length() + 1];
				strcpy_s(cFirst, vecStr[0].length() + 1, vecStr[0].c_str());
				nprob = StringToInt(vecStr[1]);
				InsertFirstWord(cFirst, nprob);
			} else {
				vecStr = SplitStr(str);
				if (vecStr.size() != 2) {
					cout << "read file error" << endl;
					return;
				}
				cSecond = new char[vecStr[0].length() + 1];
				strcpy_s(cSecond, vecStr[0].length() + 1, vecStr[0].c_str());
				nprob = StringToInt(vecStr[1]);
				InsertSecondWord(cFirst, cSecond, nprob);
				delete[] cSecond;
			}
		}
		delete[] cFirst;
	}

	void CreateNew() {
		string str;
		char* cFirst = NULL;
		char* cSecond = NULL;
		int nprob;
		fstream fin("BigramSourceNew.txt");
		if (!fin) {
			cout << "open file error" << endl;
			exit(1);
		}
		vector<string> vecStr;
		vector<string> vecSecond;
		getline(fin, str);
		cout << str << endl;
		while (getline(fin, str)) {
			if (str == "1") {
				delete[] cFirst;
				getline(fin, str);
				vecStr = SplitStr(str);
				nprob = StringToInt(vecStr[vecStr.size() - 1]);
				vecStr.pop_back();
				InsertFirstWordNew(vecStr, nprob);
			} else {
				vecSecond = SplitStr(str);
				nprob = StringToInt(vecSecond[vecSecond.size() - 1]);
				vecSecond.pop_back();
				InsertSecondWordNew(vecStr, vecSecond, nprob);
			}
		}
	}

	/*
	void CreateTree() {
		string str;
		char* cstr;

		fstream fin("chsource.txt");
		if (!fin) {
			cout << "open file error" << endl;
			exit(1);
		}
		int index = 0;
		while (getline(fin, str)) {
			if (index % 3 == 1) {
				cstr = new char[str.length() + 1];
				strcpy_s(cstr, str.length() + 1, str.c_str());
				InitInsert(cstr);
				delete[] cstr;
			}
			index++;
		}
		fin.close();

		fin.open("word.txt");
		if (!fin) {
			cout << "open file error" << endl;
			exit(1);
		}
		while (getline(fin, str)) {
			cstr = new char[str.length() + 1];
			strcpy_s(cstr, str.length() + 1, str.c_str());
			InitInsert(cstr);
			delete[] cstr;
		}
		fin.close();

		fin.open("result.txt");
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
		fin.close();
	}
	*/

	bool FindWord(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode;
		char* ctemp;
		for (int i = 0; i < strlen(cstr) - 3; i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidStateObO(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			}
			else {
				delete[] ctemp;
				return 0;
			}
		}
		ctemp = GetSubStr(cstr, strlen(cstr) - 3, 3);
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode != NULL) {
			delete[] ctemp;
			return 1;
		}
		delete[] ctemp;
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
				delete[] ctemp;
				return NULL;
			}
		}
		ctemp = GetSubStr(cstr, strlen(cstr) - 3, 3);
		tempNode = cur->FindNode(ctemp);
		if (tempNode != NULL) {
			delete[] ctemp;
			return tempNode;
		}
		delete[] ctemp;
		return NULL;
	}

	CBigramNode* GetMidState(char* cstr) {
		CBigramNode* cur = m_pRoot;
		CBigramNode* tempNode = NULL;
		char* ctemp;
		for (int i = 0; i < strlen(cstr); i = i + 3) {
			ctemp = GetSubStr(cstr, i, 3);
			tempNode = cur->FindMidState(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			}
			else {
				delete[] ctemp;
				return NULL;
			}
		}
		return tempNode;
	}

	/*
	void SetProb() {
		CBigramNode* cur = m_pRoot;
		CBigramNode** child;
		stack<CBigramNode*> staNode;
		staNode.push(cur);
		while (!staNode.empty()) {
			cur = staNode.top();
			staNode.pop();
			child = cur->m_pChild;
			if (cur->m_nState == 1) {
				cur->m_nTransPorb = (-1) * log(static_cast<double>(cur->m_nCount) / static_cast<double>(m_nTotal));
				cur->SetProb();
			}
			else {
				for (int i = 0; i < cur->m_nSize; i++) {
					staNode.push(child[i]);
				}
			}
		}
	}
	*/

	void SortTree() {
		m_pRoot->SortTree();
	}

};

/*
int main() {
	Sleep(5000);
	int n = 500000;
	int** x = new int*[n];
	STestNode** point = new STestNode*[n];
	for (int i = 0; i < n; i ++) {
		
		point[i] = new STestNode();
		x[i] = new int[1];
		
	}
	CBigramNode* bigram = new CBigramNode();
	cout << &(*bigram) << endl;
	cout << &(bigram->m_pChinese) << endl;

	Sleep(5000);

    ofstream fout("logNew.txt");
    fout << "start the process" << endl;
    fout.close();
	CBigramTree* biGramTree = new CBigramTree();
	cout << sTOTALNODE << endl;
	cout << sTOTALSIZE << endl;
	string str;
	char* cstr;
	cout << " input " << endl;
	while (cin >> str) {
		cstr = new char[str.length() + 1];
		strcpy_s(cstr, str.length() + 1, str.c_str());
		if (biGramTree->FindWord(cstr)) {
			cout << "yes" << endl;
		}
		else {
			cout << "No" << endl;
		}
		delete[] cstr;
	}
	
	return 0;
}
*/