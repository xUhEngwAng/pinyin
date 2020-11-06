#ifndef PREDICT_H_
#define PREDICT_H_

#include <cmath>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class node{
public:
    float prob;
    string pred;
    node(): prob(0), pred(""){}
};

class Predictor{
private:
    const float epsilon = 1e-8;
    const float lambda  = 0.95;
    const float w       = 0.99;
    const float Inf     = 0x7fffffff;

    unordered_map<string, float> polyMap;
    unordered_map<string, int> priorMap;
    unordered_map<string, int> posteriorMap;
    unordered_map<string, int> posterior3Map;
    unordered_map<string, vector<string>> pinyin2han;

public:
    Predictor(int mode);
    string predict2(const string &input);
    string predict3(const string &input);
};

#endif
