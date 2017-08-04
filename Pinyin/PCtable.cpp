#include<iostream>
#include<string>
#include<fstream>
#include<set>
#include<map>
#include<vector>
using namespace std;


class CHPY {
public:
    string ch_str;
    vector<string> py_vec;
    int ch_count;
    CHPY() {
        ch_str = "";
        ch_count = 0;
    }
    void set_CH (string str) {
        ch_str = str;
    }
    void set_PY (vector<string> str) {
        py_vec = str;
    }
    void insert_PY (string str) {
        py_vec.push_back(str);
    }
    void set_count (int count) {
        ch_count = count;
    }
    void printValue() {
        cout << ch_str << " ";
        for (auto it = py_vec.begin(); it != py_vec.end(); it ++) {
            cout << *it << " ";
        }
        cout << ch_count << endl;
    }
};

map<string, int> count_CH(string str) {
    fstream fin(str);
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    int count = 0;
    string sentence;
    string temp;
    map<string, int> chinese;
    while (getline(fin, sentence)) {
        for (int i = 0; i < sentence.length(); ) {
            temp = sentence.substr(i, 3);
            auto it = chinese.find(temp);
            if (it != chinese.end()) {
                chinese[temp] ++;
            } else {
                chinese[temp] = 1;
            }
            i = i + 3;
        }
    }
    fin.close();
    return chinese;
}


vector<string> split(string strline) {
    vector<string> str_res;
    int pos = 0;
    for (int i = 0; i < strline.length(); i ++) {
        if (strline[pos] == ' ') {
            pos ++;
        } else if (strline[i] == ' ') {
            str_res.push_back(strline.substr(pos, i - pos));
            pos = i + 1;
        } else if (i == strline.length() - 1) {
            str_res.push_back(strline.substr(pos, i - pos + 1));
        }
    }
    return str_res;
}


vector<CHPY> CPTable() {
    string datafile = "result.txt";
    string tablefile = "CPTable.txt";
    map<string, int> ch = count_CH(datafile);
    fstream fin(tablefile);
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    vector<string> strline;
    string s, temp;
    map<string, vector<string>> cptable;
    while (getline(fin, s)) {
        strline = split(s);
        temp = strline[0];
        strline.erase(strline.begin());
        cptable[temp] = strline;
    }
    fin.close();
    vector<CHPY> cp_set;
    CHPY cp;
    for (auto it = ch.begin(); it != ch.end(); it ++) {
        cp.set_CH(it->first);
        cp.set_count(it->second);
        vector<string> temp_vec = cptable[it->first];
        cp.set_PY(temp_vec);
        cp_set.push_back(cp);
    }
    return cp_set;
}

bool findCH(vector<CHPY> cp_set, string str) {
    for (auto it = cp_set.begin(); it != cp_set.end(); it ++) {
        if ((*it).ch_str == str) {
            return 1;
        }
    }
    return 0;
}

string strPlus(string str) {
    int pos = str.length() - 1;
    while (pos >= 0) {
        if (str[pos] + 1 <= '9') {
            str[pos] = str[pos] + 1;
            break;
        }
        str[pos] = '0';
        if (pos == 0) {
            str = '1' + str;
        }
        pos --;
    }
    return str;
}

set<string> pinYinTotal(vector<CHPY> cp_set) {
    set<string> pinyin;
    for (auto it = cp_set.begin(); it != cp_set.end(); it ++) {
        for (int i = 0; i < (*it).py_vec.size(); i ++) {
            pinyin.insert((*it).py_vec[i]);
        }
    }
    return pinyin;
}

map<string, string> pinyinID (set<string> pinyin) {
    map<string, string> pyid;
    string id = "100";
    for (auto it = pinyin.begin(); it != pinyin.end(); it ++) {
        pyid[*it] = id;
        id = strPlus(id);
    }
    return pyid;
}

map<string, map<string, int>> chineseID (map<string, string> pyid, vector<CHPY> cp_table) {
    map<string, map<string, int>> chid;
    pair<string, int> ch;
    string id;
    for (auto it = cp_table.begin(); it != cp_table.end(); it ++) {
        for (int i = 0; i < it->py_vec.size(); i ++) {
            id = pyid[(it->py_vec)[i]];
            chid[id][it->ch_str] = it->ch_count / it->py_vec.size();
        }
    }
    return chid;
}

int main() {
    map<int, map<int, int>> test;
    test[1][2] = 3;
    test[1][3] = 5;
    for (auto it = test.begin(); it != test.end(); it ++) {
        cout << it->first << ": " << endl;
        for (auto i = it->second.begin(); i != it->second.end(); i ++) {
            cout << "      " << i->first << " " << i->second;
            cout << endl;
        }
    }
    vector<CHPY> cp_set = CPTable();
    cout << cp_set.size() << endl;
    set<string> pytotal = pinYinTotal(cp_set);
    map<string, string> pyid = pinyinID(pytotal);
    cout << pyid.size() << endl;
    map<string, map<string, int>> chid = chineseID(pyid, cp_set);
    cout << chid.size() << endl;
    for (auto it = chid.begin(); it != chid.end(); it ++) {
        cout << it->first << ": " << endl;
        for (auto i = it->second.begin(); i != it->second.end(); i ++) {
            cout << "      " << i->first << " " << i->second;
            cout << endl;
        }
    }
    return 0;
  //  set<string> pinyin = pinYinTotal();
   // for (auto it = pinyin.begin(); it != pinyin.end(); it ++) {
     //   cout << *it << endl;
    //}
    //cout << pinyin.size() << endl;
}
    /*
    string str = "result.txt";
    string cpfile = "CPTable.txt";
    set<string> pinyin;
    vector<CHPY> cp_set = CPTable(str, cpfile);
    string ch;
    int count = 0;
    for (auto it = cp_set.begin(); it != cp_set.end(); it ++) {
        for (int i = 0; i < (*it).py_vec.size(); i ++) {
            pinyin.insert((*it).py_vec[i]);
        }
    }
    cout << pinyin.size() << endl;
    cout << "input chinese" << endl;
    while (cin >> ch) {
        if (findCH(cp_set, ch)) {
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }
}
*/