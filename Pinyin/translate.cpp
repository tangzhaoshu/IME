#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <stack>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include <algorithm>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include "Bigram.cpp"
using namespace std;

string UTF8ToGBK1(const string& strUTF8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    unsigned short *wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP,0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
    //strUTF8 = szGBK;
    std::string strTemp(szGBK);
    delete[]szGBK;
    delete[]wszGBK;
    return strTemp;
}

class Path {
public:
    int flag;  //0:表示当前路径无效
    int state;  //表示当前拼接状态，0表示非拼接，1表示上个词为拼接所得
    double _prob;
    vector<string> _path;
    CHNode *last;
    Path() {
        flag = 1;
        _prob = 0;
        last = NULL;
        state = 0;
    }

    void set (double p, CHNode *l) {
        _prob = p;
        last = l;
    }

    ~Path(){
        last = NULL;
        delete[] last;
    }
};

class TransNode{
public:
    int minSeg; 
    string trans_res;
    double prob;
    vector<Path*> path_total;
    TransNode() {
        minSeg = 0; // 最小切分数
        trans_res = "";
        prob = 0;
    }
    void set(string str) {
        trans_res = str;
    }

    void insertPath(Path *p) {
        if (p->flag == 0) {
            path_total.push_back(p);
            return;
        }
        if (prob == 0) {
            path_total.push_back(p);
            prob = p->_prob;
            minSeg = p->_path.size();
            return;
        }
        Path *temp;
        for (int i = 0; i < path_total.size(); i ++) {
            if (p->last == path_total[i]->last) {
                if (p->_prob < path_total[i]->_prob) {
                    temp = path_total[i];
                    path_total[i] = p;
                    delete temp;
                    if (p->_prob < prob) {
                        minSeg = p->_path.size();
                        prob = p->_prob;
                    }
                    return;
                } else if (fabs(p->_prob - path_total[i]->_prob) < 0.0000001) {
                    if (path_total[i]->_path.size() > p->_path.size()) {
                        temp = path_total[i];
                        path_total[i] = p;
                        delete temp;
                        minSeg = p->_path.size();
                        return;
                    }
                } else {
                    return;
                }
            }
        }
        path_total.push_back(p);
        if (p->_prob < prob) {
            prob = p->_prob;
            minSeg = p->_path.size();
        } else if (fabs(p->_prob - prob) < 0.0000001) {
            if (p->_path.size() < minSeg) {
                minSeg = p->_path.size();
            }
        }
    }

    void printlog() {
        ofstream fout ("log.txt", ofstream::app);
        fout << "汉字：" << trans_res << " Prob: " << prob << " 最小拼接数： " << minSeg << endl;
        fout << "路径：" << endl;
        for (int i = 0; i < path_total.size(); i ++) {
            if (path_total[i]->flag == 1) {
                for (int j = 0; j < path_total[i]->_path.size(); j ++) {
                    fout << path_total[i]->_path[j] << " ";
                }
                fout << " Prob: " << path_total[i]->_prob << endl;
            }
        }
        fout << endl;
        fout.close();
    }

    ~TransNode() {
        for (int i = 0; i < path_total.size(); i ++) {
            delete path_total[i];
        }
    }
};


int cmp(const TransNode *x, const TransNode *y) {
    if (x->minSeg < y->minSeg) {
        return 1;
    } else if (x->minSeg > y->minSeg) {
        return 0;
    }
    return x->prob < y->prob;
}

class YJNode{
public:
    string yinjie;
    int MinSplit;
    vector<TransNode*> trans_Total;
    map<string, YJNode*> next;
    YJNode() {
        MinSplit = 0;
        yinjie = "";
    }

    ~YJNode() {
        for (auto ite = next.begin(); ite != next.end(); ite ++) {
            delete ite->second;
        }
        for (int i = 0; i < trans_Total.size(); i ++) {
            delete trans_Total[i];
        }
    }

    void InsertTrans(TransNode* t) {
        if (MinSplit == 0) {
            MinSplit = t->minSeg;
        } else if (t->minSeg < MinSplit) {
            MinSplit = t->minSeg;
        }
        trans_Total.push_back(t);
        return;
    }


    TransNode* getMaxProb() {
        TransNode *temp;
        if (trans_Total.size() > 0)  {
            temp = trans_Total[0];
        }
        for (int i = 1; i < trans_Total.size(); i ++) {
            if (trans_Total[i]->prob < temp->prob) {
                temp = trans_Total[i];
            }
        }
        return temp;
    }



    void Filter() {
        int limit = 1000;
        sort(trans_Total.begin(), trans_Total.end(), cmp);
        int size = trans_Total.size();
        cout << size << endl;
        if (size > limit) {
            for (int i = 0; i < size - limit; i ++) {
                delete trans_Total[trans_Total.size() - 1];
                trans_Total.pop_back();
            }
        }
    }

    void print() {

        vector<pair<string, double>> result;
        
        for (int i = 0; i < trans_Total.size(); i ++){
            trans_Total[i]->printlog();
        }
    }

};

class TranslateTree{
public:
    YJNode* head;
    CHTree* chtree;
    
    TranslateTree() {
        head = new YJNode();
        chtree = new CHTree();
    }

    void insert (YJNode *cur, vector<string> ch, string str) {
        YJNode *yjtemp = new YJNode();
        CHNode * chtemp;
        TransNode *trans;
        Path *path;
        int limit = 1000;
        set<double> Minset;
        if (cur == head) {  
            for (int i = 0; i < ch.size(); i ++) {
                chtemp = chtree->match(ch[i]);
                trans = new TransNode();
                path = new Path();
                trans->set(chtemp->ch);
                path->set(chtemp->word_prob, chtemp);
                path->_path.push_back(ch[i]);
                trans->insertPath(path);
                yjtemp->InsertTrans(trans);
            }
            yjtemp->yinjie = str;
            cur->next[str] = yjtemp;
        } else {
            for (int i = 0; i < ch.size(); i ++) {     //遍历所有汉字
                // 遍历上次输入的汉字翻译结果
                for (auto tr = cur->trans_Total.begin(); tr != cur->trans_Total.end(); tr ++) {
                    /*
                    if (Minset.size() > limit) {   // 定义最大堆来控制遍历次数
                        auto maxite = Minset.end();
                        maxite --;
                        if ((*tr)->prob > *maxite) {
                            continue;
                        }
                    }
                    */
                    trans = new TransNode();
                    double min = 10000000;
                    //遍历路径寻找最小概率路径
                    for (int j = 0; j < (*tr)->path_total.size(); j ++) {
                        chtemp = (*tr)->path_total[j]->last;
                        auto cur_ch = chtemp->nextword.find(ch[i]);
                        if (cur_ch != chtemp->nextword.end()) {
                  //          cout << UTF8ToGBK1(cur_ch->second->ch) << " ";
                            if ((*tr)->path_total[j]->flag == 1) {
                                path = new Path();
                                path->_path = (*tr)->path_total[j]->_path;
                                path->_path.push_back(ch[i]);
                                path->_prob = (*tr)->path_total[j]->_prob + cur_ch->second->trans_prob;
                                path->last = chtree->match(ch[i]);
                                trans->insertPath(path);
                            }
                            if (cur_ch->second->flag == 1) {
                                path = new Path();
                                path->_path = (*tr)->path_total[j]->_path;
                                path->_path.pop_back();
                                path->_path.push_back(cur_ch->second->ch);
                                path->_prob = (*tr)->path_total[j]->_prob + cur_ch->second->word_prob 
                                            - chtemp->word_prob; 
                                path->last = cur_ch->second;
                                trans->insertPath(path);
                            } else {
                                path = new Path();
                                path->_path = (*tr)->path_total[j]->_path;
                                path->_path.pop_back();
                                path->_path.push_back(cur_ch->second->ch);
                                path->_path = (*tr)->path_total[j]->_path;
                                path->_prob = (*tr)->path_total[j]->_prob + cur_ch->second->word_prob 
                                            - chtemp->word_prob;
                                path->last = cur_ch->second;
                                path->flag = 0;
                                trans->insertPath(path);
                            }
                        } else {
                            if ((*tr)->path_total[j]->flag == 1) {
                                if ((*tr)->path_total[j]->state == 0) {
                                    chtemp = chtree->match(ch[i]);
                                    path = new Path();
                                    path->_path = (*tr)->path_total[j]->_path;
                                    path->_path.push_back(ch[i]);
                                    path->_prob = (*tr)->path_total[j]->_prob + chtemp->word_prob;
                                    path->last = chtemp;
                                    path->state = 1;
                                    trans->insertPath(path);
                                }
                            }
                            
                        }
                    }
   //                 if (trans->path_total.size() != 0) {
     //                   trans->trans_res = (*tr)->trans_res + ch[i];
       //                 yjtemp->trans_Total.push_back(trans);
         //           } else {
           //             delete trans;
             //       }
                    if (trans->minSeg > cur->MinSplit + 2) {
                        delete trans;
                    } else {
                        if (Minset.size() > limit) {
                            auto maxite = Minset.end();
                            maxite --;
                            if (trans->path_total.size() != 0 && trans->prob < *maxite) {
                                Minset.erase(maxite);
                                Minset.insert(trans->prob);
                                trans->trans_res = (*tr)->trans_res + ch[i];
                                yjtemp->InsertTrans(trans);
                            } else {
                                delete trans;
                            }
                        } else {
                            if (trans->path_total.size() != 0) {
                                Minset.insert(trans->prob);
                                trans->trans_res = (*tr)->trans_res + ch[i];
                                yjtemp->InsertTrans(trans);
                            } else {
                                delete trans;
                            }
                        }
                    }
                    
                }
            }
        }
        SYSTEMTIME sys;
        GetLocalTime(&sys);
        printf("%4d/%02d/%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
        cout << endl;
        yjtemp->Filter();
        yjtemp->yinjie = str;
        cur->next[str] = yjtemp;
    }



    vector<YJNode*> getTranslate(vector<vector<string>> pyseg, vector<vector<string>> candCH) {
        vector<YJNode*> result;
        YJNode *cur;
        for (int i = 0; i < pyseg.size(); i ++) {
            cur = head;
            for (int j = 0; j < pyseg[i].size(); j ++) {
                auto ite = cur->next.find(pyseg[i][j]);
                if (ite != cur->next.end()) {
                    cur = cur->next[pyseg[i][j]];
                } else {
                    insert(cur, candCH[i], pyseg[i][j]);
                    cur = cur->next[pyseg[i][j]];
                }
            }
            cout << cur->trans_Total.size() << endl;
            result.push_back(cur);
        }
        return result;
    }

    void DeleteYJ(vector<vector<string>> yjpath) {
        YJNode* cur;
        YJNode* temp;
        for (int i = 0; i < yjpath.size(); i ++) {
            cur = head;
            for (int j = 0; j < yjpath[i].size() - 1; j ++) {
                cur = cur->next[yjpath[i][j]];
            }
            temp = cur->next[yjpath[i].back()];
            auto ite = cur->next.find(yjpath[i].back());
            cur->next.erase(ite);
            delete temp;
        }
    }

    void ShowTreePath() {
        ofstream fout ("log.txt", ofstream::app);
        fout << "---------------------------音节路径------------------------" << endl;
        stack<YJNode*> nodestack;
        stack<int > step;
        int steptemp = 0;
        YJNode* temp;
        nodestack.push(head);
        step.push(0);
        while(!nodestack.empty()) {
            steptemp = step.top() + 1;
            for (int i = 0; i < step.top(); i ++) {
                fout << "   ";
            }
            step.pop();
            temp = nodestack.top();
            nodestack.pop();
            fout << temp->yinjie << endl;
            for (auto ite = temp->next.rbegin(); ite != temp->next.rend(); ite ++) {
                nodestack.push(ite->second);
                step.push(steptemp);
            }
        }
        fout << "---------------------------音节路径------------------------" << endl;
        fout << endl;
        fout.close();
    }

};
/*
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
                
/*                    if (pychnode->ch_Total.size() == 0) {
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
                    if (pychnode->ch_Total.size() == 0) {
                        process.push_back(cur);
                        i --;
                        cur = head;
                    } else {
                        cur->next[pinyin[i]] = pychnode;
                        cur = cur->next[pinyin[i]];
                    }
                    
                }
            }
        }

        process.push_back(cur);
        string comp = "";
        double compprob = 0;
        if (process.size() > 1) {
            for (int i = 0; i < process.size(); i ++) {
                comp += process[i]->getMaxProb()->chinese;
                compprob += process[i]->getMaxProb()->prob;
            }
            result[comp] = compprob;
        }
        for (int i = 0; i < process[0]->ch_Total.size(); i ++) {
            result[process[0]->ch_Total[i]->chinese] = process[0]->ch_Total[i]->prob;
        }
       */





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
//int cmpYJNode(const pair<string, double> &x, const pair<string, double> &y) {
//    return x.second < y.second;
//}


/*
int main() {
    ofstream fout("log.txt");
    fout << "开始程序" << endl;
    fout.close();

    CHPYTable *cptable = new CHPYTable();
    PYTree *pytree = new PYTree(cptable->py);
    CHTree *chtree = new CHTree();
    TranslateTree *tran = new TranslateTree();

    string input;
    cout << "input pinyin" << endl;
    



    YJNode *res;
    vector<vector<YJNode*>> step;
    vector<YJNode*> temp;
    vector<vector<string>> seg;
    char ch;
    while (1){
        
        while (!kbhit()){
        }
        ch = getch();
        if (27 == ch) {
            break;
        }
        if (8 == ch) {
            if (input.size() > 1) {
                input = input.substr(0, input.size() - 1);
            } else {
                input = "";
                system("cls");
                continue;
            }
        } else {
            input += ch;
        }
        system("cls");
        cout << input << endl;

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

        SYSTEMTIME sys;
        GetLocalTime(&sys);
        printf("%4d/%02d/%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
        cout << endl;

        for (int i = 0; i < seg.size(); i ++) {
            res = tran->getPinyinSeg(seg[i], cptable, pytree);
            temp.push_back(res);
        }

        GetLocalTime(&sys);
        printf("%4d/%02d/%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
        cout << endl;

        vector<pair<string, double>> output;
        int flag;
        for (int i = 0; i < temp.size(); i ++) {
            temp[i]->print();
            flag = 0;
            for (int j = 0; j < temp[i]->trans_Total.size(); j ++) {
                if (temp[i]->trans_Total[j]->flag == 1) {
                    output.push_back(make_pair(temp[i]->trans_Total[j]->trans_res, 
                    temp[i]->trans_Total[j]->prob));
                    flag = 1;
                } else {
                    if (flag == 0) {
                        output.push_back(make_pair(temp[i]->trans_Total[j]->trans_res, 
                        temp[i]->trans_Total[j]->prob));
                        flag = 1;
                    }
                }
            }
        }
        sort(output.begin(), output.end(), cmpYJNode);
        for(int i = 0; i < output.size(); i ++) {
            cout << UTF8ToGBK(output[i].first) <<  "Prob: " << output[i].second<< endl;
        }
        step.push_back(temp);
        temp.clear();
    }
    return 0;
}
*/