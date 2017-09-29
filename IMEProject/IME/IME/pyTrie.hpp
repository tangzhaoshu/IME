#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <fstream>
#include <stack>
#include <set>
using namespace std;

class PYNode {
public:
    int flag;       //0：非全拼，1：全拼
    string pinyin;
    PYNode *next[26];
    PYNode() {
        flag = 0;
        pinyin = "";
        for (int i = 0; i < 26; i ++) {
            next[i] = NULL;
        }
    }
	~PYNode() {
		for (int i = 0; i < 26; i++) {
			delete next[i];
		}
	}
};

class PYTree{
public:
    PYNode* root;
    PYTree() {
        root = new PYNode();
		creat();
    }
    PYTree(set<string> py) {
        root = new PYNode();
        creat();
    }

	~PYTree(){
		delete root;
	}

    void insert(string str) {
        PYNode* cur = root;
        for (int i = 0; i < str.size(); i ++) {
            if (!cur->next[str[i] - 'a']) {
                PYNode *next = new PYNode();
                cur->next[str[i] - 'a'] = next;
            }
            cur = cur->next[str[i] - 'a'];
        }
        cur->pinyin = str;
        cur->flag = 1;
    }

    bool match(string str) {
        PYNode* cur = root;
        for (int i = 0; i < str.size(); i ++) {
            if (!cur->next[str[i] - 'a']) {
                return 0;
            } else {
                cur = cur->next[str[i] - 'a'];
            }
        }
        return 1;
    }

	void creat() {
		fstream fin("pysource.txt");
		if (!fin) {
			cout << "open file error" << endl;
			exit(1);
		}
		string str;
		int index = 0;
		while (getline(fin, str)) {
			if (index == 0) {
				insert(str);
				index++;
			}
			else if (index == 1) {
				index--;
			}
		}
		fin.close();
	}

    vector<string> getpy(string str) {
        PYNode* cur = root;
        vector<string> py;
        for (int i = 0; i < str.size(); i ++) {
            if (!cur->next[str[i] - 'a']) {
                return py;
            } else {
                cur = cur->next[str[i] - 'a'];
            }
        }
        if (cur->flag == 1) {
            py.push_back(cur->pinyin);
        } else {
            queue<PYNode*> qu;
            PYNode* temp;
            for (int i = 0; i < 26; i ++) {
                if (cur->next[i] != NULL) {
                    qu.push(cur->next[i]);
                }
            }
            while (!qu.empty()) {
                temp = qu.front();
                qu.pop();
                if (temp->flag == 1) {
                    py.push_back(temp->pinyin);
                }
                for (int i = 0; i < 26; i ++) {
                    if (temp->next[i] != NULL) {
                        qu.push(temp->next[i]);
                    }
                }
            }
        }
        return py;
    }

	vector<string> GetSpecial(string str) {
		PYNode* cur = root;
		vector<string> py;
		if (str == "a" || str == "o" || str == "e") {
			py.push_back(str);
			cur = cur->next[str[0] - 'a'];
			queue<PYNode*> qu;
			PYNode* temp;
			for (int i = 0; i < 26; i++) {
				if (cur->next[i] != NULL) {
					qu.push(cur->next[i]);
				}
			}
			while (!qu.empty()) {
				temp = qu.front();
				qu.pop();
				if (temp->flag == 1) {
					py.push_back(temp->pinyin);
				}
				for (int i = 0; i < 26; i++) {
					if (temp->next[i] != NULL) {
						qu.push(temp->next[i]);
					}
				}
			}
			return py;
		}
		else {
			return getpy(str);
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

    int getState (string str) {
        PYNode* cur = root;
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

    vector<vector<string>> segment(string str) {
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
                state = getState(str.substr(pos, len));
            }
            if (state < 2) {
                if (flag == 1 || len == 1) {
                    continue;
                }
                if (!posst.empty()) {
                    temp_pos = posst.top();
                    temp_len = lenst.top();
                    if (getState(str.substr(temp_pos, temp_len + len - 1)) == 0) {
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
        ofstream fout("log.txt", ofstream::app);
        fout << str << "音节切分如下:" << endl;
        for (int i = 0; i < seg.size(); i ++) {
            fout << "切分线路" << i << ": ";
            for (int j = 0; j < seg[i].size(); j ++) {
                fout << seg[i][j] << "'";
            }
            fout << endl;
        }
        fout.close();
        return filter(seg);
    }
};
