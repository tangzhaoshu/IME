#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<fstream>
#include<cmath>
using namespace std;

class CEntryNode {
public:
    int flag;
    map<string, CEntryNode*> next;
    CEntryNode() {
        flag = 0;
    }
};

class CEntryTree {
public:
    CEntryNode* root;
    string datafile = "word.txt";

    CEntryTree () {
        root = new CEntryNode();
        Creat();
    }

    void Insert(string str) {
        CEntryNode* cur = root;
        for (int i = 0; i < str.length(); ) {
            auto ite = cur->next.find(str.substr(i, 3));
            if (ite == cur->next.end()) {
                CEntryNode *nextpoint = new CEntryNode();
                (cur->next)[str.substr(i, 3)] = nextpoint;
            }
            cur = cur->next[str.substr(i, 3)];
            i = i + 3;
        }
        cur->flag = 1;
    }

    void Creat() {
        fstream fin(datafile);
        if (!fin) {
            cout << "open file error" << endl;
            exit(1);
        }
        string str;
        while (getline(fin, str)) {
            Insert(str);
        }
        fin.close();
    }

    bool Find(string str){
        CEntryNode* cur = root;
        for (int i = 0; i < str.length(); i = i + 3) {
            auto it = cur->next.find(str.substr(i, 3));
            if (it != cur->next.end()) {
                cur = cur->next[str.substr(i, 3)];
            } else {
                return 0;
            }
        }
        if (cur->flag == 1) {
            return 1;
        } else {
            return 0;
        }
    }
};