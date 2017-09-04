#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<stack>
#include<fstream>
#include <cstdlib>
#include<cmath>
#include "Entry.cpp"
using namespace std;

class CHNode {
public:
    string ch;
    double word_count;
    double gram_count;
    double trans_prob;
    double word_prob;
    int flag;  //1:表示当前为词条， 0表示为非此条
    int end;  //1：表示为终点，0表示后续还有词条出现.
    map<string, CHNode*> nextword;

    CHNode() {
        flag = 0;
        end = 0;
        ch = "";
        gram_count = 0;
        word_count = 0;
        trans_prob = 0;
        word_prob = 0;
    }

    void PrintLog() {
        ofstream fout ("log.txt", ofstream::app);
		fout << ch << " 词条个数." << word_count << endl;
        fout.close();
        for (auto ite = nextword.begin(); ite != nextword.end(); ite ++) {
            ite->second->PrintLog();
        }
    }
};

class CHTree {
public:
    double total;
    CHNode *root;
    CEntryTree *entryTree;
    string datafile = "result.txt";

    CHTree() {
        total = 0;
        root = new CHNode();
        entryTree = new CEntryTree();
        biGramTree();
        PrintLog();
    }

    void PrintLog() {
        ofstream fout ("log.txt", ofstream::app);
        fout << "总词条个数：" << total << endl;
        stack<CHNode*> nodestack;
        CHNode* temp;
        nodestack.push(root);
        while(!nodestack.empty()) {
            temp = nodestack.top();
            nodestack.pop();
            if (temp->flag == 1) {
                fout << temp->ch << "  次数： " << temp->word_count << "  概率： " <<
                temp->word_prob << endl;
            }
            for (auto ite = temp->nextword.begin(); ite != temp->nextword.end(); ite ++) {
                nodestack.push(ite->second);
            }
        }
        fout.close();
    }

    void insert(string str) {
        CHNode* cur = root;
        for (int i = 0; i < str.length(); ) {
            auto ite = cur->nextword.find(str.substr(i, 3));
            if (ite == cur->nextword.end()) {
                CHNode *next = new CHNode();
                (cur->nextword)[str.substr(i, 3)] = next;
            }
            cur = cur->nextword[str.substr(i, 3)];
            i = i + 3;
        }
        if (str.size() == 3 || entryTree->Find(str) == 1) {
            cur->flag = 1;
            cur->word_count ++;
            total ++;
        }
        cur->ch = str;
        cur->gram_count ++;
    }

    void getProb(CHNode* root) {
        CHNode* cur = root;
        for (auto ite = cur->nextword.begin(); ite != cur->nextword.end(); ite ++) {
            ite->second->trans_prob = log(ite->second->gram_count / cur->gram_count) * (- 1);
            ite->second->word_prob = log(ite->second->gram_count / total) * (- 1);
            getProb(ite->second);
        }
    }

    void biGramTree() {
        fstream fin(datafile);
        if (!fin) {
            cout << "open file error" << endl;
            exit(1);
        }
        string str;
        while (getline(fin, str)) {
            for (int i = 0; i < str.length(); i = i + 3) {
                for (int j = i; j < str.length(); j = j + 3) {
                        insert(str.substr(i, j - i + 3));
                }
            }
        }
        fin.close();

        map<string, CHNode*> cur = root->nextword;
        for (auto ite = cur.begin(); ite != cur.end(); ite ++) {
            ite->second->word_prob = log(ite->second->word_count / total) * (- 1);
            getProb(ite->second);
        }
    }


    CHNode* match(string str){
        CHNode *cur = root;
        for (int i = 0; i < str.length(); i = i + 3) {
            auto it = cur->nextword.find(str.substr(i, 3));
            if (it != cur->nextword.end()) {
                cur = cur->nextword[str.substr(i, 3)];
            } else {
                return cur;
            }
        }
        return cur;
    }

};



/*
int main() {
    ofstream fout("log.txt");
    fout << "开始程序" << endl;
    fout.close();
    CHTree *tree = new CHTree();
    auto it = tree->root->nextword.begin();
    cout << it->first << " " << it->second->word_prob << endl;
    string str;
    cout << "input" << endl;
    while(cin >> str){
        CHNode *res = tree->match(str);
        cout << res->ch << " " << res->word_prob << " " << res->gram_count << endl;
    }
    return 0;
}
*/
