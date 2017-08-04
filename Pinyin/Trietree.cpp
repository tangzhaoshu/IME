#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Pctable.cpp"
using namespace std;

class Node {
public:
    int count_word;
    int count_total;
    Node *next[26];
    map<string, int> value;
    Node() {
        count_total = 0;
        count_word = 0;
        for (int i = 0; i < 26; i ++) {
            next[i] = NULL;
        }
    }
    void setvalue(string str, int num) {
        value[str] = num;
    }
};

void insert(Node* root, CHPY cp) {
    Node* cur = root;
    string str;
    for (auto it = cp.py_vec.begin(); it != cp.py_vec.end(); it ++) {
        str = *it;
        for (int i = 0; i < str.size(); i ++) {
            if (!cur->next[str[i] - 'a']) {
                Node *next = new Node;
                cur->next[str[i] - 'a'] = next;
            }
            cur = cur->next[str[i] - 'a'];
            cur->count_total ++;
        }
        cur->count_word ++;
        cur->setvalue(cp.ch_str, cp.ch_count);
    }
}

map<string, int> match(Node* root, string str) {
    Node* cur = root;
    map<string, int> res;
    for (int i = 0; i < str.size(); i ++) {
        if (!cur->next[str[i] - 'a']) {
            return res;
        } else {
            cur = cur->next[str[i] - 'a'];
        }
    }
    return cur->value;
}

Node* creat(string s1, string s2) {
    vector<CHPY> cp_data = CPTable(s1, s2);
    Node* root = new Node;
    for (auto it = cp_data.begin(); it != cp_data.end(); it ++) {
        insert(root, *it);
    }
    return root;
}
/*
int main() {
    string file1 = "result.txt";
    string file2 = "CPTable.txt";
    Node* root = creat(file1, file2);
    string str;
    cout << "input pinyin" << endl;
    cin >> str;
    map<string, int> result = match(root, str);
    for (auto it = result.begin(); it != result.end(); it ++) {
        cout << it->first << " " << it->second << endl;
    }
    return 0;
}
*/