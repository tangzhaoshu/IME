#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

using namespace std;


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

char getNumber(char c) {
    if (c > 96 && c < 100) {
        return '2';
    } else if (c > 99 && c < 103) {
        return '3';
    } else if (c > 102 && c < 106) {
        return '4';
    } else if (c > 105 && c < 109) {
        return '5';
    } else if (c > 108 && c < 112) {
        return '6';
    } else if (c > 111 && c < 116) {
        return '7';
    } else if (c > 115 && c < 119) {
        return '8';
    } else if (c > 118 && c < 123) {
        return '9';
    }
}

void getFull() {
    fstream fin("CPTable.txt");
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    string str;
    vector<string> strline;
    map<string, vector<string>> cpmap;
    string temp;
    while (getline(fin, str)) {
        strline = split(str);
        temp = strline[0];
        strline.erase(strline.begin());
        cpmap[temp] = strline;
    }
    fin.close();

    fin.open("testdata.txt");
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    vector<string> result;
    cout << "a" << endl;
    string spell;
    while (getline(fin, str)) {
        spell = "";
        for (int i = 0; i < str.length(); ) {
            temp = str.substr(i, 3);
            auto ite = cpmap.find(temp);
            if (ite != cpmap.end()) {
                spell += ite->second.front();
            }
            i = i + 3;
        }
        cout << spell << endl;
        result.push_back(spell);
    }
    fin.close();
    ofstream fout("inputFull.txt");
    for (int i = 0; i < result.size(); i ++) {
        fout << result[i] << endl;
    }
    fout.close();
}

void getNine(){
    fstream fin("inputFull.txt");
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    string str;
    vector<string> result;
    string temp;
    while (getline(fin, str)) {
        temp = "";
        cout << str << " " << str.length() << endl;
        for (int i = 0; i < str.length(); i ++) {
            temp += getNumber(str[i]);
        }
        result.push_back(temp);
    }
    fin.close();
    ofstream fout("inputNine.txt");
    for (int i = 0; i < result.size(); i ++) {
        fout << result[i] << endl;
    }
    fout.close();
}


void getShort() {
    fstream fin("CPTable.txt");
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    string str;
    vector<string> strline;
    map<string, vector<string>> cpmap;
    string temp;
    while (getline(fin, str)) {
        strline = split(str);
        temp = strline[0];
        strline.erase(strline.begin());
        cpmap[temp] = strline;
    }
    fin.close();

    fin.open("testdata.txt");
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    vector<string> result;
    string spell;
    while (getline(fin, str)) {
        spell = "";
        for (int i = 0; i < str.length(); ) {
            temp = str.substr(i, 3);
            auto ite = cpmap.find(temp);
            if (ite != cpmap.end()) {
                spell += ite->second.front()[0];
            }
            i = i + 3;
        }
        cout << spell << endl;
        result.push_back(spell);
    }
    fin.close();
    ofstream fout("inputShort.txt");
    for (int i = 0; i < result.size(); i ++) {
        fout << result[i] << endl;
    }
    fout.close();
}

int main() {
    getNine();
    return 1;
}