#include<iostream>
#include<string>
#include<fstream>
#include<set>
#include<map>
#include<vector>
using namespace std;


//汉字拼音统计
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
//统计汉字次数
class CHPYTable {
public:
    vector<CHPY> cp_table;
    int total_ch;
    int total_py;
    string datafile = "result.txt";
    string mapfile = "CPTable.txt";
    set<string> py;
    map<string, string> pyID;
    map<string, vector<string>> chID;

    CHPYTable() {
        total_ch = 0;
        total_py = 0;
        CPTable();
        setpinyin();
        setpinyinID();
        setchineseID();
        PrintLog();
    }

    void PrintLog() {
        ofstream fout ("log.txt", ofstream::app);
        fout << "总字数: " << total_ch << "; 总拼音数：" << total_py << endl;
        for (auto it = pyID.begin(); it != pyID.end(); it ++) {
            fout << it->first << " " << it->second << endl;
        }
        for (auto it = chID.begin(); it != chID.end(); it ++) {
            fout << it->first << "汉字如下：" << endl;
            for (int i = 0; i < it->second.size(); i ++) {
                fout << it->second[i] << " ";
            }
            fout << endl;
        }
        fout.close();
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

//根据句子集，将汉字与拼音进行对应。
    void CPTable() {
        fstream fin(datafile);
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
                total_ch ++;
            }
        }
        fin.close();

        fin.open(mapfile);
        if (!fin) {
            cout << "open file error" << endl;
            exit(1);
        }
        vector<string> strline;
        map<string, vector<string>> cpmap;
        while (getline(fin, sentence)) {
            strline = split(sentence);
            temp = strline[0];
            strline.erase(strline.begin());
            cpmap[temp] = strline;
        }
        fin.close();

        CHPY cp;
        for (auto it = chinese.begin(); it != chinese.end(); it ++) {
            cp.set_CH(it->first);
            cp.set_count(it->second);
            vector<string> temp_vec = cpmap[it->first];
            cp.set_PY(temp_vec);
            cp_table.push_back(cp);
        }
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

    void setpinyin () {
        for (auto it = cp_table.begin(); it != cp_table.end(); it ++) {
            for (int i = 0; i < (*it).py_vec.size(); i ++) {
                py.insert(((*it).py_vec)[i]);
            }
        }
    }

    void setpinyinID () {
        string id = "100";
        for (auto it = py.begin(); it != py.end(); it ++) {
            pyID[*it] = id;
            id = strPlus(id);
        }
        total_py = pyID.size();
    }

    void setchineseID () {
        string id;
        
        for (auto it = cp_table.begin(); it != cp_table.end(); it ++) {
            for (int i = 0; i < it->py_vec.size(); i ++) {
                id = pyID[(it->py_vec)[i]];
                chID[id].push_back(it->ch_str);
            }
        }
    }

    vector<string> getChinese(vector<string> pystr) {
        vector<string> chstr;
        string id;
        vector<string> temp;
        for (int i = 0; i < pystr.size(); i ++) {
            id = pyID[pystr[i]];
            temp = chID[id];
            for (int j = 0; j < temp.size(); j ++) {
                chstr.push_back(temp[j]);
            }
        }
        return chstr;
    }

};




/*
int main() {
    CHPYTable *cptable = new CHPYTable();
    cout << cptable->total_ch << "  " << cptable->total_py << endl;
    map<string, string> pyid = cptable->pyID;
    cout << pyid.size() << endl;
    for (auto it = pyid.begin(); it != pyid.end(); it ++) {
        cout << it->first << ":  " << it->second << endl;
    } 
    map<string, vector<string>> chid = cptable->chID;
    cout << chid.size() << endl;
    for (auto it = chid.begin(); it != chid.end(); it ++) {
        cout << it->first << ": " << endl;
        for (int i = 0; i < it->second.size(); i ++) {
            cout << "    " << it->second[i] << endl;
        }
    }
    return 0;
  //  set<string> pinyin = pinYinTotal();
   // for (auto it = pinyin.begin(); it != pinyin.end(); it ++) {
     //   cout << *it << endl;
    //}
    //cout << pinyin.size() << endl;
}
*/
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