#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<stack>
#include<fstream>
#include<cmath>
#include "Entry.cpp"
using namespace std;


//int TOTAL = 0;
//int SUM_SIZE = 0;
class CHNode {
public:
    string ch;
    int word_count;
    int gram_count;
    int trans_prob;
    int word_prob;
    int flag;  //1:表示当前为词条， 0表示为非此条
    map<string, CHNode*> nextword;

    CHNode() {
        flag = 0;
        ch = "";
        gram_count = 0;
        word_count = 0;
        trans_prob = 0;
        word_prob = 0;
    }

    void PrintLog() {
        //ofstream fout ("log.txt", ofstream::app);
        //fout << ch << " 词条个数：" << word_count << endl;
        //fout.close();
    //    TOTAL ++;
    //    SUM_SIZE += ch.capacity();
        for (auto ite = nextword.begin(); ite != nextword.end(); ite ++) {
            ite->second->PrintLog();
        }
    }
};


class CHTree {
public:
    int total;
    int SUM = 0;
    CHNode *root;
    CEntryTree *entryTree;
    string datafile = "result.txt";

    CHTree() {
        total = 0;
        root = new CHNode();
  //      cout << sizeof(root) << endl;
        
        entryTree = new CEntryTree();
        biGramTree();
    //    cout << SUM << endl;

   //     cout << root->nextword.size() << endl;
        PrintLog();
    //    cout << TOTAL << endl;
   //     cout << SUM_SIZE << endl;
    }

    void PrintLog() {
    //    root->PrintLog();
        
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
                SUM ++;
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
            double temp = static_cast<double>(ite->second->gram_count) / static_cast<double>(cur->gram_count);
            ite->second->trans_prob = log(temp) * (-1);
            if (ite->second->trans_prob < 0) {
                ite->second->trans_prob = 0;
            }
      //      
            temp = static_cast<double>(ite->second->gram_count) / static_cast<double>(total);
            ite->second->word_prob = log(temp) * (- 1);
      //      if (ite->second->word_prob < 0) {
        //        cout << ite->second->trans_prob << " " << cur->gram_count << endl;
         //   }
            getProb(ite->second);
        }
    }

    bool judge(string str) {
        for (int i = 3; i < str.size(); ) {
            if (str.substr(0, i).size() == 3 || entryTree->Find(str.substr(0, i)) == 1) {
                if (i == str.size()) {
                    return 1;
                } 
                if (str.substr(i, str.size()- i).size() == 3 || entryTree->Find(str.substr(i, str.size() - i)) == 1) {
                    return 1;
                }
            }
            i = i + 3;
        }
        return 0;
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
                    if (str.substr(i, j - i + 3).size() == 3 || judge(str.substr(i, j - i + 3))) {
                        insert(str.substr(i, j - i + 3));
                    }
                    //insert(str.substr(i, j - i + 3));

                }
            }
        }
        fin.close();

        map<string, CHNode*> cur = root->nextword;
        for (auto ite = cur.begin(); ite != cur.end(); ite ++) {
            double temp = static_cast<double>(ite->second->word_count) / static_cast<double>(total);
            ite->second->word_prob = log(temp) * (- 1);
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
    bool Find(string str){
        CHNode *cur = root;
        for (int i = 0; i < str.length(); i = i + 3) {
            auto it = cur->nextword.find(str.substr(i, 3));
            if (it != cur->nextword.end()) {
                cur = cur->nextword[str.substr(i, 3)];
            } else {
                return 0;
            }
        }
        return 1;
    }

};


/*
int main() {
    ofstream fout("log.txt");
    fout << "开始程序" << endl;
    fout.close();
    CHTree *tree = new CHTree();
    string str;
    cout << "input" << endl;
    while(cin >> str){
        CHNode *res = tree->match(str);
 //       cout << res->ch << " " << res->word_prob << " " << res->gram_count << endl;
    }
    return 0;
}
*/