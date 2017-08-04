#include<iostream>
#include<string>
#include<fstream>
using namespace std;

int main() {
    string filename = "test.tex";
    string str;
    ifstream fin(filename);
    ofstream fout("result.txt");
    if (!fin) {
        cout << "unable to open file" << endl;
        exit(1);
    }
    if (!fout) {
        cout << "unable to open file" << endl;
        exit(1);
    }
    int count = 0;
    while (getline(fin, str)) {
        if (str.length() > 6) {
            fout << str << endl;
        }
    }
    fin.close();
    fout.close();
    return 0;
}