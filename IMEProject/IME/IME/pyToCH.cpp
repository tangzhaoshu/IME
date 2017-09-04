#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <queue>
using namespace std;








/*
int main() {
    vector<CHPY> cp_set = CPTable();
    map<string, string> pyid = pinyinID(cp_set);
    map<string, vector<string>> chid = chineseID(pyid, cp_set);
    Node* root = creat(pyid);
    cout << "input pinyin" << endl;
    string str;
    vector<string> ch;
    while (cin >> str) {
        vector<string> id = getID(root, str);
        ch = IDtoCH(id, chid);
        for (int i = 0; i < ch.size(); i ++) {
            cout << ch[i] << " ";
        }
        cout << endl;
    }
    return 0;
}
*/