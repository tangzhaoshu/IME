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
    MultiByteToWideChar(CP_UTF8, 0, (LPCCH)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

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
    int flag;  //0:表示当前路径无效，词条中间状态
    int gram;  //当前路径的gram次数
    int joint; //当前路径的拼接次数
    int state;  //表示当前拼接状态，0表示非拼接，1表示上个词为拼接所得
    double _prob;
    vector<string> _path;
    CHNode *last;
    Path() {
        joint = 0;
        gram = 0;
        flag = 1;
        _prob = 0;
        last = NULL;
        state = 0;
    }
    Path(const Path *p) {
        flag = p->flag;
        gram = p->gram;
        joint = p->joint;
        state = p->state;
        _prob = p->_prob;
        _path = p->_path;
        last = p->last;
    }

    void popAndPush(string str) {
        _path.pop_back();
        _path.push_back(str);
    }

    void push(string str) {
        _path.push_back(str);
    }

    void set (double p, CHNode *l) {
        _prob = p;
        last = l;
    }

    void addGram() {
        gram ++;
    }

    void addJoint() {
        joint ++;
    }

    void setstate(int s, int f) {
        state = s;
        flag = f;
    }

    ~Path(){
        last = NULL;
        delete[] last;
    }
};

int cmpPath(const Path *x, const Path *y) {
    if (x->gram + x->joint + 1 < y->gram + y->joint) {
        return 1;
    } else if (x->gram + x->joint > y->gram + y->joint + 1) {
        return 0;
    } else {
        if (x->joint < y->joint) {
            return 1;
        } else if (x->joint > y->joint){
            return 0;
        } else {
            return x->_prob < y->_prob;
        }
    }
}

class TransNode{
public:
    int minSeg; 
    string trans_res;
    double prob;
    vector<Path*> path_total;
    TransNode() {
        minSeg = 0; // 最小词条个数
        trans_res = "";
        prob = 0;
    }
    void set(string str) {
        trans_res = str;
    }

    void setMinNum() {
        if (path_total.size() == 0) {
            minSeg = 0;
            prob = 0;
            return;
        }
        sort(path_total.begin(), path_total.end(), cmpPath);
        int i = 0;
        while (i < path_total.size() && path_total[i]->flag == 0) {
            i ++;
        }
        if (i < path_total.size()) {
            minSeg = path_total[i]->_path.size();
            prob = path_total[i]->_prob;
        } else {
            minSeg = 0;
            prob = 0;
        }
    }

    void deletePath(Path *p) {
        Path *temp;
        for (auto ite = path_total.begin(); ite != path_total.end(); ite ++) {
            if (*ite == p) {
                temp = *ite;
                ite = path_total.erase(ite);
                delete temp;
                setMinNum();
                return;
            }
        }
    }

    void insertPath(Path *p) {   //节点重合时合并路径
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
                if (p->_prob - path_total[i]->_prob < -0.0000001) {
                    temp = path_total[i];
                    path_total[i] = p;
                    delete temp;
                    if (p->_prob - prob < -0.000001) {
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
        if (p->_prob - prob < -0.000001) {
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
        fout << "汉字  " << trans_res << " Prob  " << prob << " 最小词条数  " << minSeg << endl;
        fout << "路径" << endl;
        for (int i = 0; i < path_total.size(); i ++) {
            if (path_total[i]->flag == 1) {
                fout << "有效路径 ";
                for (int j = 0; j < path_total[i]->_path.size(); j ++) {
                    fout << path_total[i]->_path[j] << " ";
                }
                fout << " Prob: " << path_total[i]->_prob << ";  ";
                fout << " Ngram次数  " << path_total[i]->gram << " 拼接次数  " << path_total[i]->joint << endl;
            } else {
                fout << "无效路径  ";
                for (int j = 0; j < path_total[i]->_path.size(); j ++) {
                    fout << path_total[i]->_path[j] << " ";
                }
                fout << " Prob  " << path_total[i]->_prob << ";  ";
                fout << " Ngram次数   " << path_total[i]->gram << " 拼接次数   " << path_total[i]->joint << endl;
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
	if (x->minSeg == 1 && y->minSeg > 1) {
		return 1;
	}
	if (y->minSeg == 1 && x->minSeg > 1) {
		return 0;
	}
	if (x->minSeg + 1 < y->minSeg) {
		return 1;
	}
	else if (x->minSeg > y->minSeg + 1) {
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

    void deleteTrans(TransNode *t) {
        if (trans_Total.size() == 0) {
            return;
        }
        TransNode *temp = NULL;
        if (trans_Total.size() == 1) {
            temp = trans_Total[0];
            trans_Total.clear();
            delete temp;
            MinSplit = 0;
        } else {
            for (auto ite = trans_Total.begin(); ite != trans_Total.end(); ite ++) {
                if (*ite == t) {
                    ite = trans_Total.erase(ite);
                    delete temp;
                    return;
                }
            }
        }
    }

    void InsertTrans(TransNode* t) {
    //    cout << 3 << UTF8ToGBK1(t->trans_res) << endl;
        if (t->minSeg == 0) {
            trans_Total.push_back(t);
            return;
        }
        if (MinSplit == 0) {
            MinSplit = t->minSeg;
            trans_Total.push_back(t);
            return;
        } else if (t->minSeg > MinSplit + 1) {
            return;
        }
        vector<Path*> waitDelete;
        int index = 0;
        Path *temp;
        while (index < t->path_total.size()) {
            
            if (t->path_total[index]->flag == 0) {
                index ++;
                continue;
            }
            int flag = 0;
            for (int i = 0; i < trans_Total.size(); i ++) {
                if (flag == 1) {
                    break;
                }
                for (int j = 0; j < trans_Total[i]->path_total.size(); j ++) {
                    temp = trans_Total[i]->path_total[j];
                    if (temp->flag == 1 && t->path_total[index]->last == temp->last) {
           //             cout << UTF8ToGBK1(t->path_total[index]->last->ch) << " "
            //            << UTF8ToGBK1(trans_Total[i]->trans_res) << 3 << endl;
                        if (cmpPath(t->path_total[index], trans_Total[i]->path_total[j])) {
                            if (trans_Total[i]->path_total.size() == 1) {
                                deleteTrans(trans_Total[i]);
                            } else {
                                trans_Total[i]->deletePath(temp);
                            }
                        } else {
                            if (t->path_total.size() == 1) {
                                delete t;
                                return;
                            } else {
                                waitDelete.push_back(t->path_total[index]);
                            }
                        }
                        flag = 1;
                        break;
                    }
                }
            }
            index ++;
        }
        for (int i = 0; i < waitDelete.size(); i ++) {
            t->deletePath(waitDelete[i]);
        }
        if (t->path_total.size() == 0) {
            return;
        }
        if (t->minSeg == 0) {
            trans_Total.push_back(t);
            return;
        }
        if (MinSplit == 0 ) {
            MinSplit = t->minSeg;
        } else if (t->minSeg < MinSplit) {
            MinSplit = t->minSeg;
        }
        trans_Total.push_back(t);
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
        int limit = 3000;
        sort(trans_Total.begin(), trans_Total.end(), cmp);
        int size = trans_Total.size();
        int index = 1;
        for (auto ite = trans_Total.begin(); ite != trans_Total.end(); ite ++) {
            if (index > limit) {
                trans_Total.erase(ite, trans_Total.end());
                break;
            } else {
                index += (*ite)->path_total.size();
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
        int limit = 3000;
        int g = 0;
        int c = 0;
        int joint = 0;
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
       //         cout << "字： " << UTF8ToGBK1(ch[i]) << endl;
                int optFlag = 1;
                // 遍历上次输入的汉字翻译结果
                for (auto tr = cur->trans_Total.begin(); tr != cur->trans_Total.end(); tr ++) {
                    trans = new TransNode();
                    //遍历路径寻找最小概率路径
              //      cout << UTF8ToGBK1((*tr)->trans_res) << " ";
                    for (int j = 0; j < (*tr)->path_total.size(); j ++) {
                        chtemp = (*tr)->path_total[j]->last;
                        auto cur_ch = chtemp->nextword.find(ch[i]);
                        if (cur_ch != chtemp->nextword.end()) {

                //            cout << 1 << UTF8ToGBK1(cur_ch->second->ch) << endl;

                            if (cur_ch->second->flag == 1) {
                                path = new Path((*tr)->path_total[j]);
                                path->_path.pop_back();
                                double p = 0.0;
                                double p2, p1;
                                if (path->_path.size() > 0) {
                                    if (chtree->Find(path->_path.back() + chtemp->ch)) {
                                        p1 = chtree->match(path->_path.back() + chtemp->ch)->trans_prob;
                                        if (chtree->Find(path->_path.back() + cur_ch->second->ch)) {
                                            CHNode *c = chtree->match(path->_path.back() + cur_ch->second->ch);
                                            p2 = log(c->gram_count / chtree->match(path->_path.back())->gram_count) * (-1);
                                        } else {
                                            p2 = cur_ch->second->word_prob;
                                            path->gram --;
                                            path->joint ++;
                                        }
                                    }
                                } else {
                                    p1 = chtemp->word_prob;
                                    p2 = cur_ch->second->word_prob;
                                }
                                p = p + p2 - p1;
                                path->_path.push_back(cur_ch->second->ch);
                            //    
                           //     double p1 = (*tr)->path_total[j]->_prob + cur_ch->second->word_prob 
                           //     - chtemp->word_prob;
                           //     double p = 0.0;
                           //     if (path->_path.size() > 0) {
                           //         CHNode *c = chtree->match(path->_path.back() + cur_ch->second->ch);
                           //         double p2 = log(c->gram_count / chtree->match(path->_path.back())->gram_count) * (-1);
                           //         if (p2 < p1) {
                            //           p1 = p2;
                             //      }
                            //    }
                            //    
                        //        path->popAndPush(cur_ch->second->ch);
                                path->set((*tr)->path_total[j]->_prob + p, cur_ch->second);
                                path->setstate(0, 1);
                                trans->insertPath(path);
                            } else {
                                if (chtree->entryTree->Find(cur_ch->second->ch) != 2) {
                                    path = new Path((*tr)->path_total[j]);
                                    path->_path.pop_back();
                                    double p = 0.0;
                                    double p2 = 0.0;
                                    double p1 = 0.0;
                                    if (path->_path.size() > 0) {
                                        
                                        if (chtree->Find(path->_path.back() + chtemp->ch)) {
                                            p1 = chtree->match(path->_path.back() + chtemp->ch)->trans_prob;
                                            if (chtree->Find(path->_path.back() + cur_ch->second->ch)) {
                                                CHNode *c = chtree->match(path->_path.back() + cur_ch->second->ch);
                                                p2 = log(c->gram_count / chtree->match(path->_path.back())->gram_count) * (-1);
                                                if (p2 < 0) {
                                                    cout << UTF8ToGBK1(path->_path.back()) << endl;
                                                }
                                            } else {
                                                p2 = cur_ch->second->word_prob;
                                                path->gram --;
                                                path->joint ++;
                                            }
                                        }
                                    } else {
                                        p1 = chtemp->word_prob;
                                        p2 = cur_ch->second->word_prob;
                                    }
                                    p = p + p2 - p1;
                                    
                                    path->_path.push_back(cur_ch->second->ch);
                                    path->set((*tr)->path_total[j]->_prob + p, cur_ch->second);
                                    path->setstate(0, 0);
                                    trans->insertPath(path);
                                }
                            }
                            if ((*tr)->path_total[j]->flag == 1) {
                                path = new Path((*tr)->path_total[j]);
                                path->push(ch[i]);
                                path->set((*tr)->path_total[j]->_prob + cur_ch->second->trans_prob, chtree->match(ch[i]));
                                path->addGram();
                                path->setstate(0, 1);
                                trans->insertPath(path);
                                optFlag == 0;
                            }
                        } else {
                            if (optFlag == 0) {
                                continue;
                            }
                            if ((*tr)->path_total[j]->flag == 1) {
                                if ((*tr)->path_total[j]->state == 0) {
                                    chtemp = chtree->match(ch[i]);
                                    path = new Path((*tr)->path_total[j]);
                                    path->push(ch[i]);
                                    path->set((*tr)->path_total[j]->_prob + chtemp->word_prob, chtemp);
                                    path->addJoint();
                                    path->setstate(0, 1);
                                    trans->insertPath(path);
                                    optFlag = 0;
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
    //    cout << g << " " << c << " " << joint << endl;
   //     SYSTEMTIME sys;
    //    GetLocalTime(&sys);
    //    printf("%4d/%02d/%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
        yjtemp->Filter();
        yjtemp->yinjie = str;
        cur->next[str] = yjtemp;
    }

    void FilterTranslate(vector<YJNode*> total) {
        if (total.size() == 0) {
            return;
        }
        vector<vector<TransNode*>::iterator> ite;
        int limit = 3000;
        for (int i = 0; i < total.size(); i ++) {
            ite.push_back(total[i]->trans_Total.begin());
        }
        int pos;
        for (int i = 0; i < limit; i ++) {
            pos = -1;
            for (int j = 0; j < ite.size(); j ++) {
                if (ite[j] == total[j]->trans_Total.end()) {
                    continue;
                }
                if (pos == -1) {
                    pos = j;
                } else {
                    if (cmp(*(ite[pos]), *(ite[j])) == 0) {
                        pos = j;
                    }
                }
            }
            if(pos == -1) {
                break;
            }
            i += (*ite[pos])->path_total.size() - 1;
            ite[pos] ++;
        }
        for (int i = 0; i < total.size(); i ++) {
            total[i]->trans_Total.erase(ite[i], total[i]->trans_Total.end());
        }
    }

    vector<YJNode*> getTranslate(vector<vector<string>> pyseg, vector<vector<string>> candCH) {
        vector<YJNode*> result;
        YJNode *cur;
        for (int i = 0; i < pyseg.size(); i ++) {
            cur = head;
            for (int j = 0; j < pyseg[i].size() - 1; j ++) {
                auto ite = cur->next.find(pyseg[i][j]);
                if (ite != cur->next.end()) {
                    cur = cur->next[pyseg[i][j]];
                } else {
                    break;
           //         insert(cur, candCH[i], pyseg[i][j]);
             //       cur = cur->next[pyseg[i][j]];
                }
            }
            auto ite = cur->next.find(pyseg[i].back());
            if (ite != cur->next.end()) {
                cur = cur->next[pyseg[i].back()];
            } else {
                insert(cur, candCH[i], pyseg[i].back());
                cur = cur->next[pyseg[i].back()];
            }
            result.push_back(cur);
        }
        
        SYSTEMTIME sys;
        GetLocalTime(&sys);
        printf("%4d/%02d/%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
        cout << endl;

        FilterTranslate(result);
        int sum = 0;
        for (int i = 0; i < result.size(); i ++) {
            sum += result[i]->trans_Total.size();
        }
        cout << sum << endl;
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