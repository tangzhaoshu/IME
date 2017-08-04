#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Pctable.cpp"
using namespace std;

class Node {
public:
    int flag;       //0：非全拼，1：全拼
    string id;
    Node *next[26];
    map<string, int> value;
    Node() {
        flag = 0;
        id = "";
        for (int i = 0; i < 26; i ++) {
            next[i] = NULL;
        }
    }
};

void insert(Node* root, string str) {
    Node* cur = root;
    for (int i = 0; i < str.size(); i ++) {
        if (!cur->next[str[i] - 'a']) {
            Node *next = new Node;
            cur->next[str[i] - 'a'] = next;
        }
        cur = cur->next[str[i] - 'a'];
    }
    cur->flag = 1;
}

bool match(Node* root, string str) {
    Node* cur = root;
    for (int i = 0; i < str.size(); i ++) {
        if (!cur->next[str[i] - 'a']) {
            return 0;
        } else {
            cur = cur->next[str[i] - 'a'];
        }
    }
    return 1;
}

Node* creat() {
    vector<CHPY> chpy = CPTable();
    set<string> pinyin = pinYinTotal(chpy);
    Node* root = new Node;
    for (auto it = pinyin.begin(); it != pinyin.end(); it ++) {
        insert(root, *it);
    }
    return root;
}




/*
int main() {
    Node* root = creat();
    cout << "input pinyin" << endl;
    string str;
    cin >> str;
    if (match(root, str)) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }
    return 0;
}
*/