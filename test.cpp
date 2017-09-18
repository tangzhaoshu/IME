#include <iostream>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <process.h>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <time.h>
using namespace std;


class CHNode {
public:
    string ch;
    int trans_prob;
    int word_prob;
    int flag;  //1:表示当前为词条， 0表示为非此条
    map<string, CHNode*> nextword;

    CHNode() {
        flag = 0;
        ch = "hellow wor奥斯卡的福利卡时间到了副科级lasdfasdfasdfasdfasfasdfd";
    }

    void PrintLog() {
        //ofstream fout ("log.txt", ofstream::app);
        //fout << ch << " 词条个数：" << word_count << endl;
        //fout.close();
    }
};

class TestNode{
public:
    string str;
    TestNode(string s) {
        str = s;
    }
    ~TestNode(){
        cout << "delete  " << str << endl;
    }
};

int GetDirection()
{
    int ret = 0;
 
    do 
    {
        int ch = getch();
        if(isascii(ch))
            continue;
 
        ch = getch();
        switch(ch)
        {
        case 72:   
            ret = 2; // top
            break;
        case 75:   
            ret = 1; // left 
            break;
        case 77:   
            ret = 3; // right
            break;
        case 80:   
            ret = 4; // down
            break;
        default:   
            break;
        }
    } while (ret == 0);
     
    return ret;
}

string UTF8ToGBK(const string& strUTF8)
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

class CCharNode {
public:
    char* x;
    CCharNode() {
        x = new char[2];
        x[0] = 'a';
        x[1] = '\0';
    }
    ~CCharNode() {
        delete[] x;
    }
};

vector<char*> GetChar() {
    CCharNode* p = new CCharNode();
    vector<char*>  res;
    res.push_back(p->x);
    delete p;
    return res;
}

 
int main()
{   
    vector<char*> y;
    y = GetChar();
    cout << y.size() << endl;
    cout << y[0] << endl;;

    fstream fin("result.txt");
    if (!fin) {
        cout << "open file error" << endl;
        exit(1);
    }
    vector<string> result;
    string str;
    getline(fin, str);
    getline(fin, str);
    cout << str.length() << endl;
    cout << str.substr(3, 3) << endl;
    const char* cstr = str.c_str();
    cout << cstr << endl;
    cout << strlen(cstr) << endl;
    char *c1 = "123";
    char *c2 = "423";
    cout << c1 << endl;
    /*
    std::stringstream ss;
    string str;
    map<string, int> test_map;
    for (int i = 0; i < 10; i ++) {
        ss.clear();
        ss << i;
        ss >> str;
        test_map[str] = i;
    }
    cout << test_map.size() << endl;
    cout << sizeof(test_map) << endl;
    cout << sizeof(*test_map.begin()) << endl;
    */
    Sleep(10000);
    /*
    CHNode *chnode = new CHNode();
    cout << sizeof(*chnode) << endl;
    cout << chnode->ch << endl;
    string tstring = "阿斯顿阿斯顿发生尽快了解a";
    cout << sizeof(tstring) << '1' << endl;
    cout << tstring.capacity() << endl;
    vector<TestNode> test;
    map<string, TestNode*> testmap;
    cout << sizeof(testmap) << endl;
    TestNode *temp;
    temp = new TestNode("a");
    cout << sizeof(*temp) << 2333 << endl;
    test.push_back(*temp);
    temp = new TestNode("bbb");
    test.push_back(*temp);
    testmap["b"] = temp;
    cout << &test[0] << endl;
    cout << &test[1] << endl;
    cout << sizeof(test) << endl;
    cout << sizeof("a") << endl;
    map<int, int> m_i_test;
    m_i_test[1] = 1;
    map<string, string> m_s_test;
    cout << sizeof(m_i_test) << endl;
    cout << sizeof(m_s_test) << endl;

    delete temp;
    cout << "size: " << test.size() << endl;
    float a = 1.0;
    float b = 3.0;
    vector<int> te;
    te = {1, 2, 3, 4};
    cout << te.back() << endl;
    SYSTEMTIME sys;
    GetLocalTime(&sys);
    printf("%4d/%02d/%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
    cout << endl;
    GetLocalTime(&sys);
    printf("%4d/%02d/%02d %02d:%02d:%02d.%03d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds);
    cout << endl;
    */
    return 0;
}

