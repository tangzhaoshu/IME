#include <iostream>
#include <map>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

char* GetSubStr(const char* cstr, int left, int len) {
	int size = len + 1;
	char* result = new char[size];
	for (int i = left; i < left + len; i++) {
		result[i - left] = cstr[i];
	}
	result[size - 1] = '\0';
	return result;
}

char* MergeStr(const char* lstr, const char* rstr) {
	int size = strlen(lstr) + strlen(rstr) + 1;
	char* result = new char[size];
	int pos = 0;
	for (int i = 0; i < strlen(lstr); i++) {
		result[pos] = lstr[i];
		pos++;
	}
	for (int i = 0; i < strlen(rstr); i++) {
		result[pos] = rstr[i];
		pos++;
	}
	result[pos] = '\0';
	return result;
}

string UTF8ToGBK(const string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short *wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCCH)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	std::string strTemp(szGBK);
	delete[]szGBK;
	delete[]wszGBK;
	return strTemp;
}
