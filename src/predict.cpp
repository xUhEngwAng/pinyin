#include "predict.h"
#include "model.h"
using namespace std;

Predictor::Predictor(int mode = 2){
    string polyFile = "model/poly_mapping.txt";
    string priorFile = "model/prior.txt";
    string posteriorFile = "model/posterior.txt";
    string posterior3File = "model/posterior3zip2.txt";
    bulid_pinyin2han_map(pinyin2han);
    //load_from_file(polyFile, polyMap);
    load_from_file(priorFile, priorMap);
    load_from_file(posteriorFile, posteriorMap);
    if(mode == 3) load_from_file(posterior3File, posterior3Map);

    // load polyphone map
    ifstream in(polyFile);
    if(!in.is_open()){
        cerr << "Failed to open file " << polyFile << endl;
        exit(0);
    }
    string key;
    float value;
    while(in >> key >> value){
        polyMap[key] = value;
    }
    //cout << "Predictor initialization done." << endl;
}

string Predictor::predict2(const string &input){
    vector<node> curr, prev;
    string pinyin, prevHan, currHan;
    int pos, prevPos = 0, maxIdx, prior;
    float currProb, maxProb, posterior, poly_coeff;

    prev = vector<node>(1);
    while(true){
        pos = input.find(' ', prevPos);
        if(pos == string::npos) pinyin = input.substr(prevPos);
        else                   pinyin = input.substr(prevPos, pos - prevPos);
        prevPos = pos + 1;

        curr = vector<node>(pinyin2han[pinyin].size());
        for(int jx = 0; jx != pinyin2han[pinyin].size(); ++jx){
            currHan = pinyin2han[pinyin][jx];
            maxProb = -Inf;
            prior = priorMap[currHan];
            if(polyMap.find(currHan + pinyin) != polyMap.end())
                poly_coeff = polyMap[currHan + pinyin];
            else
                poly_coeff = 0.0;

            for(int ix = 0; ix != prev.size(); ++ix){
                if(prev[ix].pred.size() < 3) 
                    prevHan = "";
                else 
                    prevHan = prev[ix].pred.substr(prev[ix].pred.size() - 3);

                if(posteriorMap.find(prevHan + currHan) == posteriorMap.end()) 
                    posterior = 0.0;
                else 
                    posterior = posteriorMap[prevHan + currHan] / (float)priorMap[prevHan];
                posterior = lambda * posterior + (1 - lambda) * prior / (float)0x7fffffff;

                currProb = log(posterior + epsilon) + prev[ix].prob + poly_coeff;
                if(currProb > maxProb){
                    maxProb = currProb;
                    maxIdx  = ix;
                }
            }
            curr[jx].prob = maxProb;
            curr[jx].pred = prev[maxIdx].pred + currHan;
        }
        if(pos == string::npos) break;
        prev = curr;
    }
    maxProb = curr[0].prob;
    maxIdx = 0;
    for(int ix = 1; ix != curr.size(); ++ix){
        if(curr[ix].prob > maxProb){
            maxProb = curr[ix].prob;
            maxIdx = ix;
        }
    }
    return curr[maxIdx].pred;
}

string Predictor::predict3(const string &input){
    vector<vector<node>> curr, prev;
    string pinyin, prevPrevHan, prevHan, currHan;
    int pos, prevPos = 0, maxIdx, prior;
    float currProb, maxProb, posterior, posterior3, poly_coeff;

    prev = vector<vector<node>>(1, vector<node>(1));
    while(true){
        pos = input.find(' ', prevPos);
        if(pos == string::npos) pinyin = input.substr(prevPos);
        else                    pinyin = input.substr(prevPos, pos - prevPos);
        prevPos = pos + 1;

        curr = vector<vector<node>>(pinyin2han[pinyin].size(), vector<node>(prev.size()));
        for(int jx = 0; jx != pinyin2han[pinyin].size(); ++jx){
            currHan = pinyin2han[pinyin][jx];
            prior = priorMap[currHan];
            if(polyMap.find(currHan + pinyin) != polyMap.end())
                poly_coeff = polyMap[currHan + pinyin];
            else
                poly_coeff = 0.0;

            for(int ix = 0; ix != prev.size(); ++ix){
                maxProb = -Inf;
                if(prev[ix][0].pred.size() < 3) 
                    prevHan = "";
                else 
                    prevHan = prev[ix][0].pred.substr(prev[ix][0].pred.size() - 3);

                if(posteriorMap.find(prevHan + currHan) == posteriorMap.end()) 
                    posterior = 0.0;
                else 
                    posterior = posteriorMap[prevHan + currHan] / (float)priorMap[prevHan];
                posterior = lambda * posterior + (1 - lambda) * prior / (float)0x7fffffff;

                for(int kx = 0; kx != prev[ix].size(); ++kx){
                    if(prev[ix][kx].pred.size() < 6)
                        prevPrevHan = "";
                    else
                        prevPrevHan = prev[ix][kx].pred.substr(prev[ix][kx].pred.size() - 6, 3);
                    
                    if(posterior3Map.find(prevPrevHan + prevHan + currHan) == posterior3Map.end())
                        posterior3 = 0.0;
                    else
                        posterior3 = posterior3Map[prevPrevHan + prevHan + currHan] / (float)posteriorMap[prevHan + currHan];
                    posterior3 = w * posterior3 + (1 - w) * posterior;

                    currProb = log(posterior3 + epsilon) + prev[ix][kx].prob + poly_coeff;
                    if(currProb > maxProb){
                        maxProb = currProb;
                        maxIdx  = kx;
                    }
                }
                curr[jx][ix].prob = maxProb;
                curr[jx][ix].pred = prev[ix][maxIdx].pred + currHan;
            }
        }
        if(pos == string::npos) break;
        prev = curr;
    }
    int maxIx, maxJx;
    maxProb = -Inf;
    for(int ix = 0; ix != curr.size(); ++ix)
        for(int jx = 0; jx != curr[ix].size(); ++jx){
            if(curr[ix][jx].prob > maxProb){
                maxProb = curr[ix][jx].prob;
                maxIx = ix, maxJx = jx;
            }
        }
    return curr[maxIx][maxJx].pred;
}
