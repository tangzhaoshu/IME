#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "pyTrie.cpp"
using namespace std;


vector<vector<string>> seg_total;

void segmentRP(string str, Node* root, vector<string> seg, int pos) {
    Node* cur = root;
    for (int i = pos; i < str.size(); i ++) {
        if (!cur->next[str[i] - 'a']) {
            return ;
        } else {
            cur = cur->next[str[i] - 'a'];
            if (cur->flag == 0) {
                continue;
            } else if (i == str.length() - 1) {
                seg.push_back(str.substr(pos, i - pos + 1));
                seg_total.push_back(seg);
            } else {
                seg.push_back(str.substr(pos, i - pos + 1));
                segmentRP(str, root, seg, i + 1);
                seg.pop_back();
            }
        }
    }
}


int main() {
    Node* root = creat();
    string str;
    cout << "input pinyin" << endl;
    cin >> str;
    vector<string> temp;
    segmentRP(str, root, temp, 0);
    for (int i = 0; i < seg_total.size(); i ++) {
        for (int j = 0; j < seg_total[i].size(); j ++) {
            cout << seg_total[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}