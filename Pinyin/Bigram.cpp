#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<fstream>
#include<cmath>
using namespace std;

class CHNode {
public:
    string ch;
    double count;
    double prob;
    map<string, CHNode*> nextword;

    CHNode() {
        ch = "";
        count = 0;
        prob = 0;
    }
};

class CHTree {
public:
    CHNode *root;
    string datafile = "result.txt";

    CHTree() {
        root = new CHNode();
        biGramTree();
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
            cur->ch = str.substr(i, 3);
            i = i + 3;
        }
        cur->count ++;
    }

    void getProb(CHNode* root) {
        CHNode* cur = root;
        for (auto ite = cur->nextword.begin(); ite != cur->nextword.end(); ite ++) {
            ite->second->prob = log(ite->second->count / cur->count) * (- 1);
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
        int total = 0;
        for (auto ite = cur.begin(); ite != cur.end(); ite ++) {
            total += ite->second->count;
        }
        for (auto ite = cur.begin(); ite != cur.end(); ite ++) {
            ite->second->prob = log(ite->second->count / total) * (- 1);
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
    CHTree *tree = new CHTree();
    auto it = tree->root->nextword.begin();
    cout << it->first << " " << it->second->prob << endl;
    string str;
    cout << "input" << endl;
    while(cin >> str){
        CHNode *res = tree->match(str);
        cout << res->ch << " " << res->prob << " " << res->count << endl;
    }
    return 0;
}
*/