#include<iostream>
#include<string>
#include<fstream>
#include<set>
#include<map>
#include<vector>
#include<algorithm>
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

int cmpCHCount(const pair<string, double> &x, const pair<string, double> &y) {
    return x.second > y.second;
}

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
    map<string, vector<pair<string, int>>> chID;

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
                fout << it->second[i].first << " ";
            }
            fout << endl;
        }
        fout.close();


        fout.open("pysource.txt");
        for (auto ite = pyID.begin(); ite != pyID.end(); ite ++) {
            fout << ite->first << endl;
            fout << ite->second <<endl;
        }
        fout.close();
        fout.open("chsource.txt");
        for (auto ite = chID.begin(); ite != chID.end(); ite ++) {
            for (int i = 0; i < ite->second.size(); i ++) {
                fout << ite->first << endl;
                fout << ite->second[i].first << endl;
                fout << ite->second[i].second << endl;
            }
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
                chID[id].push_back(make_pair(it->ch_str, it->ch_count));
            }
        }
    }

    vector<string> getChinese(vector<string> pystr) {
        vector<string> chstr;
        vector<pair<string, int>> chinfo;
        string id;
        for (int i = 0; i < pystr.size(); i ++) {
            id = pyID[pystr[i]];
            for (int j = 0; j < chID[id].size(); j ++) {
                chinfo.push_back(chID[id][j]);
            }
        }
        sort(chinfo.begin(), chinfo.end(), cmpCHCount);
        for (int i = 0; i < chinfo.size(); i ++) {
            chstr.push_back(chinfo[i].first);
        }
        return chstr;
    }

};



int main() {
    CHPYTable *cptable = new CHPYTable();
  
}