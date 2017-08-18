#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <hash_map>
#include <set>
#include <hash_set>
#include <algorithm>
#include <limits>
#include <math.h>
//using namespace stdext;
using namespace std;

const int LEN = 10240;
const int BUCKET_SIZE = 1024000;

const int WORD_LEN = 10 * 2;//n个汉字 
const int SHORTEST_WORD_LEN = 2;
const int LEAST_FREQ = 3;
const float MI = 2.5;
const float H = 1.0;

int i = 0;
float g_entropy_thrhd = 1;
long article_len = 0;

struct WordInfo {
	WordInfo() : freq(0),hxx(0.0) {}
	bool calc_is_keep() {
		float h = min(calc_entropy(left_trim),calc_entropy(right_trim));
		if (h>H/*calc_is_keep_left() && calc_is_keep_right()*/) {
			return true;
		}
		return false;
		
	}
	bool calc_is_keep_left(){
		float left_entropy = calc_entropy(left_trim);
		
	}
	bool calc_is_keep_right(){
		float right_entropy = calc_entropy(right_trim);
		if (right_entropy < g_entropy_thrhd) {
			return false;
		}
		return true;
	}
	float calc_entropy(map<string, int>& trim) {
		if (trim.size() == 0) {
			return numeric_limits<float>::max();
		}
		int trim_sum = 0;
		for (map<string, int>::iterator it = trim.begin(); it != trim.end(); ++it) {
			trim_sum += it->second;
		}
		float entropy = 0.0;
		for (map<string, int>::iterator it = trim.begin(); it != trim.end(); ++it) {
			float p = static_cast<float>(it->second) / trim_sum;
			entropy -= p * log(p);
		}
		return entropy;
	}
	int freq;
	float hxx;
	map<string, int> left_trim;
	map<string, int> right_trim;
};


int gbk_char_len(const char* str)
{
	return (unsigned char)str[0] < 0x80 ? 1 : 2;
}

bool gbk_hanzi(const char* str)
{
	int char_len = gbk_char_len(str);
	if (char_len == 2 && (unsigned char)str[0] < 0xa1 || (unsigned char)str[0] > 0xa9) {
		return true;
	}
	return false;
}

int unhanzi_to_space(char* oline, const char* iline)
{
	const int line_len = strlen(iline);
	int io = 0;
	bool is_prev_hanzi = false;
	for (int ii = 0; ii < line_len;) {
		const int char_len = gbk_char_len(iline + ii);
		if (gbk_hanzi(iline + ii)) {
			memcpy(oline + io, iline + ii, char_len);
			io += char_len;
			is_prev_hanzi = true;
		}
		else {
			if (is_prev_hanzi) {
				oline[io++] = ' ';
				is_prev_hanzi = false;
			}
		}
		ii += char_len;
	}
	oline[io] = '\0';
	return 0;
}

int space_seperate_line_to_hanzi_vector(std::vector<string>* p_hzstr_vec, char* oline)
{
	char* pch = strtok(oline, " ");
	while (pch != NULL) {
		i++;
		if (i >= 1024*1024*10)
		{
			cout << i << endl;
			i = 0;
		}
		string hzstr(pch);
		//cout << hzstr << endl;
		p_hzstr_vec->push_back(hzstr);
		pch = strtok(NULL, " ");
	}
	return 0;
}

//caution: 考虑到仅仅支持gbk，又都是汉字，字符定长为2 bytes
int gen_words_freq(map<string, WordInfo> *wordinfo_map, vector<string>& hzstr_vec)
{
	int ii = 0;
	cout << hzstr_vec.size() << endl;
	for (vector<string>::iterator it = hzstr_vec.begin(); it != hzstr_vec.end(); ++it) {
		ii++;
	    if(ii > 1024*1024){
			cout <<"gen_word_freq"<< ii << endl;
			cout << "文章字数" << article_len << endl;
			ii = 0;
		}
		
		article_len += static_cast<int>(it->length());//统计文章字数
		
		
		//cout << article_len <<endl;
		int phrase_len = min(static_cast<int>(it->length()), WORD_LEN);
		
		for (int i = 2; i <= phrase_len; i += 2) {
		
			for (string::iterator ic = it->begin(); ic < it->end() - i + 2; ic += 2) {
				string tmp(ic, ic + i);
				//cout << tmp << endl;
				map<string, WordInfo>::iterator it_map = wordinfo_map->find(tmp);
				
				if (it_map == wordinfo_map->end()) {
					
					WordInfo tmp_wordinfo;
					tmp_wordinfo.freq = 1;
					(*wordinfo_map)[tmp] = tmp_wordinfo;
					
				}
				else {
					++it_map->second.freq;
					
				}
			}
		}
	}
	cout <<(*wordinfo_map).size() << endl;
	//cout << (article_len/2) << endl;
	return 0;
}

int gen_cad_words(set<string>* cad_words_set, map<string, WordInfo>& wordinfo_map)
{
	cout << article_len << endl;
	int ii = 0;
	for (map<string, WordInfo>::iterator it = wordinfo_map.begin(); it != wordinfo_map.end(); ++it) {
		ii++;
		if(ii > 1024*1024*10){
			cout <<"gen_cad_words"<< ii << endl;
			ii = 0;
		}
		int freq = it->second.freq;
		string& word = const_cast<string&>(it->first);
		WordInfo& wordinfo = it->second;
		if (word.length() <= SHORTEST_WORD_LEN) {
			continue;
		}
		if (wordinfo.freq < LEAST_FREQ) {
			continue;
		}
		float min_ff = 10000000.0;
		float art_freq = 0.0;
		for (string::iterator ic = word.begin() + 2; ic <= word.end() - 2; ic += 2) {
			string temp_left(word.begin(), ic);
			int left_f = wordinfo_map.find(temp_left)->second.freq;
			string temp_right(ic, word.end());
			int right_f = wordinfo_map.find(temp_right)->second.freq;

			float left = log((static_cast<float>(left_f)) / article_len);
			float right = log((static_cast<float>(right_f)) / article_len );
			float art_freq = log((static_cast<float>(freq)) / article_len );
			min_ff = min((art_freq - left - right), min_ff);
		}
		//cout << min_ff << endl;
		if ((min_ff) > MI) {
			cad_words_set->insert(word);
			it->second.hxx = min_ff;
		}
	}
	
	return 0;
}

int gen_trim_entropy(set<string>& cad_words_set, map<string, WordInfo>* wordinfo_map)
{
	int ii = 0;
	for (map<string, WordInfo>::iterator it = wordinfo_map->begin(); it != wordinfo_map->end(); ++it) {
		ii++;
		if(ii > 1024*1024*10){
			cout << "gen_trim_entropy"<<ii << endl;
			ii = 0;
		}
		string& word = const_cast<string&>(it->first);
		int freq = it->second.freq;
		if (word.length() >= 2 * 2) {//三个汉字，abc，插入词bc's left trim a，插入词ab's right trim c
			string left_trim(word.begin(), word.begin() + 2);
			string right_part(word.begin() + 2, word.end());
			if (cad_words_set.find(right_part) != cad_words_set.end())  {
				map<string, WordInfo>::iterator it_r = wordinfo_map->find(right_part);
				if (it_r == wordinfo_map->end()) {
					fprintf(stderr, "WARNING, word[%s] in cad word, not in word_info", right_part.c_str());
					continue;
				}
				it_r->second.left_trim[left_trim] += freq;
			}
			string right_trim(word.end() - 2, word.end());
			string left_part(word.begin(), word.end() - 2);
			if (cad_words_set.find(left_part) != cad_words_set.end()) {
				map<string, WordInfo>::iterator it_l = wordinfo_map->find(left_part);
				if (it_l == wordinfo_map->end()) {
					fprintf(stderr, "WARNING, word[%s] in cad_word, not in word_info", left_part.c_str());
					continue;
				}
				it_l->second.right_trim[right_trim] += freq;
			}
		}
	}
	return 0;
}

int trim_entropy_filter(vector<string>* keep_words, set<string>& cad_words_set, map<string, WordInfo>& wordinfo_map)
{
	keep_words->reserve(cad_words_set.size());
	for (set<string>::iterator it = cad_words_set.begin(); it != cad_words_set.end(); ++it) {
		map<string, WordInfo>::iterator it_map = wordinfo_map.find(*it);
		if (it_map == wordinfo_map.end()) {
			fprintf(stderr, "WARNING, word[%s] in cad_word, not in word_info", it->c_str());
			continue;
		}
		if (it_map->first.size() <= WORD_LEN - 4 && it_map->second.calc_is_keep()) {
			keep_words->push_back(*it);
		}
	}
	return 0;
}

int dump_to_file(vector<string>& keep_words, const char* outfile, map<string, WordInfo>& wordinfo_map)
{
	FILE* out;
	out = fopen(outfile, "w");
	if (!out) {
		cout << "输出文件打开失败！" << endl;
		return -1;
	}else cout << "输出文件打开成功！" << endl;

	for (vector<string>::iterator it = keep_words.begin(); it != keep_words.end(); ++it) {
		WordInfo& wordinfo = wordinfo_map.find(*it)->second;
		fprintf(out, "%s\n", it->c_str());
	}
	return 0;
}

int main()
{
	const char* infile = "input.txt";
	const char* outfile = "output.txt";
	FILE* in;
	in = fopen(infile,"rb");
	char line[LEN];
	char oline[LEN];
	
	vector<string> hzstr_vec;

	if (in){
		cout << "输入文件打开成功！" << endl;
		while (fgets(line,LEN, in)){
			unhanzi_to_space(oline, line);
			space_seperate_line_to_hanzi_vector(&hzstr_vec, oline);
		}
		fclose(in);
	}
	else{
		cout << "输入文件打开失败！" << endl;
	}
	
	map<string, WordInfo> wordinfo_map;
	cout << "0" << endl;
	gen_words_freq(&wordinfo_map, hzstr_vec);
	cout << "1" << endl;
	set<string> cad_words_set;
	gen_cad_words(&cad_words_set, wordinfo_map);
	cout << "2" << endl;
	gen_trim_entropy(cad_words_set, &wordinfo_map);
	cout << "3" << endl;
	vector<string> keep_words;
	trim_entropy_filter(&keep_words, cad_words_set, wordinfo_map);
	cout << "4" << endl;
	dump_to_file(keep_words, outfile, wordinfo_map);
	cout << "5" << endl;

	return 0;
}