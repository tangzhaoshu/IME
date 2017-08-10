#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include "Bigram.cpp"
#include "PCtable.cpp"
#include "pyTrie.cpp"
using namespace std;

/*
int MultiByteToWideChar(
　　UINT CodePage,
　　DWORD dwFlags,
　　LPCSTR lpMultiByteStr,
　　int cchMultiByte,
　　LPWSTR lpWideCharStr,
　　int cchWideChar
); 
string UTF8ToGBK(const std::string& strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    unsigned short * wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, NULL, NULL);
    //strUTF8 = szGBK;
    std::string strTemp(szGBK);
    delete[]szGBK;
    delete[]wszGBK;
    return strTemp;
}
*/


class CHStr{
public:
    string chinese;
    double prob;
    CHNode *last;
    CHStr() {
        chinese = "";
        prob = 0;
        last = NULL;
    }
    void set(string str, double p, CHNode *l) {
        chinese = str;
        prob = p;
        last = l;
    }
};

class PYCHNode{
public:
    string yinjie;
    vector<CHStr*> ch_Total;
    map<string, PYCHNode*> next;
    PYCHNode() {
        yinjie = "";
    }
};

class TranslateTree{
public:
    PYCHNode* head;
    
    TranslateTree() {
        head = new PYCHNode();
    }

    vector<CHStr*> getPinyinSeg(vector<string> pinyin, CHTree *chroot, CHPYTable *cptable , PYTree *pyroot) {
        
        PYCHNode* cur = head;
        for (int i = 0; i < pinyin.size(); i ++) {
            auto ite = cur->next.find(pinyin[i]);
            if (ite != cur->next.end()) {
                cur = cur->next[pinyin[i]];
            } else {
                if (cur == head) {
                    vector<string> py = pyroot->getpy(pinyin[i]);
                    vector<string> ch = cptable->getChinese(py);

                    ofstream fout ("log.txt", ofstream::app);
                    fout << pinyin[i] << "对应的汉字如下：";
                    for (int z = 0; z < ch.size(); z ++) {
                        fout << ch[z] << " ";
                    }
                    fout << endl;
                    fout.close();

                    CHStr *chstr;
                    PYCHNode *pychnode = new PYCHNode();
                    CHNode* chtemp;
                    for (int j = 0; j < ch.size(); j ++) {
                        chtemp = chroot->match(ch[j]);
                        chstr = new CHStr();
                        chstr->set(ch[j], chtemp->prob, chtemp);
                        pychnode->ch_Total.push_back(chstr);
                    }
                    cur->next[pinyin[i]] = pychnode;
                    cur = cur->next[pinyin[i]];
                } else {
                    vector<string> py = pyroot->getpy(pinyin[i]);
                    vector<string> ch = cptable->getChinese(py);

                    ofstream fout ("log.txt", ofstream::app);
                    fout << pinyin[i] << "对应的汉字如下：";
                    for (int z = 0; z < ch.size(); z ++) {
                        fout << ch[z] << " ";
                    }
                    fout << endl;
                    fout.close();

                    CHNode *chtemp;
                    CHStr *chstr;
                    PYCHNode *pychnode = new PYCHNode();
                    for (int j = 0; j < ch.size(); j ++) {
                        for (auto it = cur->ch_Total.begin(); it != cur->ch_Total.end(); it ++) {
                            chtemp = (*it)->last;
                            auto ite1 = chtemp->nextword.find(ch[j]);
                            if (ite1 != chtemp->nextword.end()) {
                                chtemp = chtemp->nextword[ch[j]];
                                chstr = new CHStr();
                                chstr->set((*it)->chinese + ch[j], (*it)->prob + chtemp->prob, chtemp);
                                pychnode->ch_Total.push_back(chstr);
                            }
                        }
                    }
                    if (pychnode->ch_Total.size() == 0) {
                        string str;
                        for (int j = 0; j < ch.size(); j ++) {
                            if (j == 0) {
                                chtemp = chroot->match(ch[j]);
                                str = ch[j];
                            } else {
                                if (chroot->match(ch[j])->prob < chtemp->prob) {
                                    chtemp = chroot->match(ch[j]);
                                    str = ch[j];
                                }
                            }
                        }
                        for (int j = 0; j < cur->ch_Total.size(); j ++) {
                            if (j == 0) {
                                chstr = cur->ch_Total[j];
                            } else {
                                if (cur->ch_Total[j]->prob < chstr->prob) {
                                    chstr = cur->ch_Total[j];
                                }
                            }
                        }
                        CHStr *tempstr = new CHStr();
                        tempstr->set(chstr->chinese + str, chstr->prob + chtemp->prob, chtemp);
                        pychnode->ch_Total.push_back(tempstr);
                    }
                    cur->next[pinyin[i]] = pychnode;
                    cur = cur->next[pinyin[i]];
                }
            }
        }
        return cur->ch_Total;
    }
};
/*
double getPorb(NodeCH* root, string str1, string str2) {
    if (str1.size() < 3) {
        return 0;
    }
    string str3 = str1.substr(str1.length() - 3, 3);
    double count1 = match(root, str1 + str2)->prob;
    return count1;
}

int getTotal(NodeCH* root, vector<string> ch) {
    int total = 0;
    for (int i = 0; i < ch.size(); i ++) {
        total += match(root, ch[i]);
    }
    return total;
}

string translate(vector<string> pinyin, NodeCH *root, map<string, vector<string>> chid, Node* pyroot) {
    map<string, double> result;
    map<string, double> temp;
    vector<string> ch = pyToCH (pinyin[0], pyroot, chid);
    int total = getTotal(root, ch);
    for (int i = 0; i < ch.size(); i ++) {
        result[ch[i]] = match(root, ch[i])->prob;
    }
    float max;
    string str;
    for (int i = 1; i < pinyin.size(); i ++) {
        temp = result;
        result.clear();
        ch = pyToCH (pinyin[i], pyroot, chid);
        for (int j = 0; j < ch.size(); j ++) {
            max = 10000000;
            str = "";
            for (auto it = temp.begin(); it != temp.end(); it ++) {
                if ((getPorb(root, it->first, ch[j]) + it->second) < max) {
                    str = it->first + ch[j];
                    max = getPorb(root, it->first, ch[j]) + it->second;
                }
            }
            if (str != "") {
                result[str] = max;
            }
        }
        if (result.empty()) {
            total = getTotal(root, ch);
            for (int j = 0; j < ch.size(); j ++) {
                max = 0.0;
                str = "";
                float prob = static_cast<float> (match(root, ch[j])) / static_cast<float> (total);
                for (auto it = temp.begin(); it != temp.end(); it ++) {
                    if (it->second * prob > max) {
                        max = it->second * prob;
                        str = it->first + ch[j];
                    }
                }
                result[str] = max;
            }
        }
    }
    max = 0.0;
    str = "";
    for (auto it = result.begin(); it != result.end(); it ++) {
        if (it->second > max) {
            max = it->second;
            str = it->first;
        }
    }
    return str;
}
*/
int main() {
    ofstream fout("log.txt");
    fout << "开始程序" << endl;
    fout.close();

    CHPYTable *cptable = new CHPYTable();
    cout << "chinese number: " << cptable->total_ch << endl;
    cout << "pinyin number: " << cptable->total_py << endl;
    string input;
    cout << "input pinyin" << endl;
    PYTree *pytree = new PYTree();
    pytree->creat(cptable->py);
    CHTree *chtree = new CHTree();
    TranslateTree *tran = new TranslateTree();



    vector<CHStr*> res;
    vector<vector<string>> seg;
    char ch;
    while (cin >> input){
        /*
        while (!kbhit()){
        }
        ch = getch();
        if (27 == ch) {
            break;
        }
        if (8 == ch) {
            if (input.size() > 0) {
                input = input.substr(0, input.size() - 1);
            }
        } else {
            input += ch;
        }
        system("cls");
        cout << input << endl;
        */

        seg = pytree->segment(input);


        ofstream fout ("log.txt", ofstream::app);
        fout << input << "拼音的最终音节切分" << endl;
        for (int i = 0; i < seg.size(); i ++) {
            fout << "切分线路" << i << ": ";
            for (int j = 0; j < seg[i].size(); j ++) {
                fout << seg[i][j] << "'";
            }
            fout << endl;
        }
        fout.close();


        for (int i = 0; i < seg.size(); i ++) {
            res = tran->getPinyinSeg(seg[i], chtree, cptable, pytree);
            cout << res.size() << endl;
            for (int j = 0; j < res.size(); j ++) {
                cout << res[j]->chinese << " " << res[j]->prob << endl;
            }
        }
    }
    return 0;
}