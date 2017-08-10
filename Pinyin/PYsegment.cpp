#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
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

int judge(string str, Node* root) {
    Node* cur = root;
    for (int i = 0; i < str.size(); i ++) {
        if (!cur->next[str[i] - 'a']) {
            return i;
        } else {
            cur = cur->next[str[i] - 'a'];
        }
    }
    return str.size();
}

void segmentSP(string str, Node* root, vector<string> seg, int pos) {
    Node* cur = root;
    int flag = 0;
    for (int i = pos; i < str.size(); i ++) {
        if (!cur->next[str[i] - 'a']) {
            if (i > pos && flag == 0) {
                if (!seg.empty()) {
                    int upsize = seg.back().size();
                    if (judge(str.substr(pos - upsize, str.size() - pos + upsize), root) >= i - pos + upsize) {
                        return;
                    }
                }
                seg.push_back(str.substr(pos, i - pos));
                segmentSP(str, root, seg, i);
            }
            return;
        } else {
            cur = cur->next[str[i] - 'a'];
            if (cur->flag == 1) {
                seg.push_back(str.substr(pos, i - pos + 1));
                segmentSP(str, root, seg, i + 1);
                seg.pop_back();
                flag = 1;
            }
        }
    }
    if (pos < str.length() && flag == 0) {
        if (!seg.empty()) {
            int upsize = seg.back().size();
            if (judge(str.substr(pos - upsize, str.size() - pos + upsize), root) >= str.length() - pos + upsize) {
                return;
            }
        }
        seg.push_back(str.substr(pos, str.length() - pos));
    }
    if (flag == 0) {
        seg_total.push_back(seg);
    }
}

vector<vector<string>> filter(vector<vector<string>> seg) {
    vector<vector<string>> res;
    int min = 100000;
    for (int i = 0; i < seg.size(); i ++) {
        if (seg[i].size() < min) {
            min = seg[i].size();
        }
    }
    for (int i = 0; i < seg.size(); i ++) {
        if (seg[i].size() < min + 2) {
            res.push_back(seg[i]);
        }
    }
    return res;
}

int getState (Node* root, string str) {
    Node* cur = root;
    if (str.size() == 0) {
        return 0;
    }
    for (int i = 0; i < str.size(); i ++) {
        if (!cur->next[str[i] - 'a']) {
            return 0;
        }
        cur = cur->next[str[i] - 'a'];
    }
    if (cur->flag == 1) {
        return 2;
    } else {
        return 1;
    }
}

vector<vector<string>> segment(string str, Node* root) {
    vector<vector<string>> seg;
    vector<string> split;
    stack<int> posst;
    stack<int> lenst;
    posst.push(0);
    lenst.push(0);
    int pos, len, temp_pos, temp_len, state, flag;
    while (!posst.empty()) {
        pos = posst.top();
        posst.pop();
        len = lenst.top();
        lenst.pop();
        flag = 0;
        state = 1;
        if (pos == str.length()) {
            seg.push_back(split);
            split.pop_back();
            posst.pop();
            lenst.pop();
            continue;
        }
        if (len > 0) {
            split.pop_back();
            flag = 1;
        }
        while (state == 1) {
            len ++;
            if (len > str.length() - pos) {
                break;
            }
            state = getState(root, str.substr(pos, len));
        }
        if (state < 2) {
            if (flag == 1 || len == 1) {
                continue;
            }
            if (!posst.empty()) {
                temp_pos = posst.top();
                temp_len = lenst.top();
                if (getState(root, str.substr(temp_pos, temp_len + len - 1)) == 0) {
                    split.push_back(str.substr(pos, len - 1));
                    posst.push(pos);
                    lenst.push(len - 1);
                    posst.push(pos + len - 1);
                    lenst.push(0);
                }
            } else {
                split.push_back(str.substr(pos, len - 1));
                posst.push(pos);
                lenst.push(len - 1);
                posst.push(pos + len - 1);
                lenst.push(0);
            }
        }
        if (state == 2) {
            split.push_back(str.substr(pos, len));
            posst.push(pos);
            lenst.push(len);
            posst.push(pos + len);
            lenst.push(0);
        }
    }
    return filter(seg);
}



/*
int main() {
    vector<CHPY> cp_set = CPTable();
    map<string, string> pyid = pinyinID(cp_set);
    Node* root = creat(pyid);
    string str;
    vector<vector<string>> seg;
    cout << "input pinyin" << endl;
    while(cin >> str) {
        seg = segment(str, root);
        for (int i = 0; i < seg.size(); i ++) {
            for (int j = 0; j < seg[i].size(); j ++) {
                cout << seg[i][j] << " ";
            }
            cout << endl;
        }
    }
    return 0;
}
*/