#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <string.h>
#include <queue>
#include "FunctionGroup.hpp"

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
	unsigned short m_nState;	//0: ��һ�������ķ���ֹ״̬��1����һ����������ֹ״̬��2���ڶ��������ķ���ֹ״̬��3���ڶ�����������ֹ״̬
	unsigned short m_nSize;
	unsigned short m_nCapacity;


	CBigramNode() {
		sTOTALNODE++;
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
		sTOTALNODE++;
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
		sTOTALNODE++;
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
		for (int i = 0; i < m_nSize; i++) {
			sTOTALSIZE += sizeof(*m_pChild[i]);
			m_pChild[i]->SumSize();
		}
	}


	//��������������ֹ�ڵ�
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

	//��������Ѱ�Ҵ�����ֹ�ڵ�
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

	//�������������м�ڵ�
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

	//�������������м�ڵ�
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


	//���������Bigramת�Ƹ��ʵ���Դ��Ϣ
	void PrintSource() {
		ofstream fout("BigramSource.txt", ofstream::app);
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

	void PrintLog() {
		ofstream fout("logNew.txt", ofstream::app);
		stack<char*> staChar;
		CBigramNode* pParent;

		if (m_nState == 1 || m_nState == 3) {
			if (m_nState == 3) {
				fout << "   ";
			}
			else {
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
	

	//���ݵ�ǰ�ڵ��ȡ���������ַ���
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
		cout << m_nCapacity << " " << m_nSize << endl;
		CBigramNode** arrayNode = m_pChild;
		m_pChild = new CBigramNode*[m_nSize];
		for (int i = 0; i < m_nSize; i++) {
			m_pChild[i] = arrayNode[i];
			arrayNode[i] = NULL;
		}
		delete[] arrayNode;
		m_nCapacity = m_nSize;
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
		Create();
	//	SumSize();
		//	SortTree();
		//SetProb();
		//	PrintLog();
	}

	~CBigramTree() {
		delete m_pRoot;
	}

	void SumSize() {
		sTOTALSIZE += sizeof(*m_pRoot);
		m_pRoot->SumSize();
	}

	void PrintSource() {
		ofstream fout("BigramSource.txt");
		fout << "BigramSource" << endl;
		fout.close();
		m_pRoot->PrintSource();
	}

	void PrintLog() {
		ofstream fout("logNew.txt", ofstream::app);
		for (auto ite = m_ChTable.begin(); ite != m_ChTable.end(); ite++) {
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
				}
				else {
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
			}
			else {
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
			}
			else {
				cout << "insert error" << endl;
				return;
			}
		}
		ctemp = GetSubStr(cFirst, strlen(cFirst) - 3, 3);
		tempNode = cur->FindNodeObO(ctemp);
		if (tempNode != NULL) {
			cur = tempNode;
			delete[] ctemp;
		}
		else {
			cout << "insert error" << endl;
			return;
		}

		for (int i = 0; i < strlen(cSecond) - 3; i = i + 3) {
			ctemp = GetSubStr(cSecond, i, 3);
			tempNode = cur->FindMidStateObO(ctemp);
			if (tempNode != NULL) {
				cur = tempNode;
				delete[] ctemp;
			}
			else {
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
			}
			else {
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


	void SortTree() {
		m_pRoot->SortTree();
	}

};