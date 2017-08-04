#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<fstream>
using namespace std;

class NodeCH {
public:
    int count;
    map<string, NodeCH*> nextword;
    NodeCH() {
        count = 0;
    }
};

void insert(NodeCH* root, string str) {
    NodeCH* cur = root;
    for (int i = 0; i < str.length(); ) {
        auto ite = cur->nextword.find(str.substr(i, 3));
        if (ite == cur->nextword.end()) {
            NodeCH *next = new NodeCH();
            (cur->nextword)[str.substr(i, 3)] = next;
        }
        cur =cur->nextword[str.substr(i, 3)];
        i = i + 3;
    }
    cur->count ++;
}

NodeCH* biGramTree(string filename) {
    fstream fin(filename);
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    string str;
    NodeCH *root = new NodeCH();
    while (getline(fin, str)) {
        for (int i = 0; i < str.length(); i = i + 3) {
            for (int j = i; j < str.length(); j = j + 3) {
                insert(root, str.substr(i, j - i + 3));
            }
        }
    }
    fin.close();
    return root;
}

int match(NodeCH* root, string str){
    NodeCH *cur = root;
    for (int i = 0; i < str.length(); i = i + 3) {
        auto it = cur->nextword.find(str.substr(i, 3));
        if (it != cur->nextword.end()) {
            cur = cur->nextword[str.substr(i, 3)];
        } else {
            return 0;
        }
    }
    return cur->count;
}

int main() {
    string file = "result.txt";
    NodeCH *root = biGramTree(file);
    string str;
    cout << "input" << endl;
    cin >> str;
    cout << match(root, str) << endl;
    return 0;
}