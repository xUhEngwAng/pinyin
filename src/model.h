#ifndef MODEL_H_
#define MODEL_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
using namespace std;

bool isASCII(char ch);
void load_valid_char(unordered_map<string, int> &prior);
void bulid_pinyin2han_map(unordered_map<string, vector<string>> &pinyin2han);
void compute_prior_posterior(const string &corpus_file, 
                             unordered_map<string, int> &prior,
                             unordered_map<string, int> &posterior,
                             unordered_map<string, int> &posterior3);
void save2file(const string &fileName, const unordered_map<string, int> &data);
void load_from_file(const string &fileName, unordered_map<string, int> &data);

// These two functions are not used in this project.
string get_body(const string &currLine);
string get_title(const string &currLine);

// path of corpus files 
const int NUM_FILE = 9;
const string path = "pinyin/data/train/";
const string corpus_files[] = {"2016-02", "2016-04", "2016-05", "2016-06", "2016-07", "2016-08",
                               "2016-09", "2016-10", "2016-11"};

#endif
