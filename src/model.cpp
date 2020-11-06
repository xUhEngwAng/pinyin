#include "model.h"

bool isASCII(char ch){
    return (ch & 0x80) == 0;
}

void load_valid_char(unordered_map<string, int> &prior){
    string validCharPath = "data/train/一二级汉字.txt";
    ifstream in(validCharPath);
    if(!in.is_open()){
        cerr << "Failed to open file " << validCharPath << endl;
        exit(0);
    }
    string content, curr;
    getline(in, content);
    for(int ix = 0; ix < content.size(); ix += 3){
        curr = content.substr(ix, 3);
        prior[curr] = 0;
    }
    in.close();
}

void bulid_pinyin2han_map(unordered_map<string, vector<string>> &pinyin2han){
    string pinyin2hanPath = "data/train/拼音汉字表.txt";
    ifstream in(pinyin2hanPath);
    if(!in.is_open()){
        cerr << "Failed to open file " << pinyin2hanPath << endl;
        exit(0);
    }
    string line, pinyin, han;
    int pos, prevPos;
    while(getline(in, line)){
        pos = line.find(' ');
        pinyin = line.substr(0, pos);
        pinyin2han[pinyin] = vector<string>();
        while(true){
            // split line by space
            if(pos == string::npos) break;
            prevPos = pos + 1;
            pos = line.find(' ', prevPos);
            if(pos == string::npos) han = line.substr(prevPos);
            else                    han = line.substr(prevPos, pos - prevPos);
            pinyin2han[pinyin].push_back(han);
        }
    }
    in.close();
}

void compute_prior_posterior(const string &corpus_file, 
                             unordered_map<string, int> &prior,
                             unordered_map<string, int> &posterior,
                             unordered_map<string, int> &posterior3){
    ifstream in(corpus_file);
    if(!in.is_open()){
        cerr << "Failed to open file " << corpus_file << endl;
        exit(0);
    }
    string line, curr, next, afterNext;
    while(getline(in, line)){
        for(int ix = 0; ix < line.size(); ){
            if(isASCII(line[ix])) {
                ix += 1;
                continue;
            }
            // update prior
            curr = line.substr(ix, 3);
            ix += 3;
            if(prior.find(curr) == prior.end()) continue;
            prior[curr] += 1;

            // update posterior
            if(isASCII(line[ix])) continue;
            next = line.substr(ix, 3);
            if(prior.find(next) == prior.end()) continue;
            posterior[curr + next] += 1;

            // update posterior3
            if(isASCII(line[ix + 3])) continue;
            afterNext = line.substr(ix + 3, 3);
            if(prior.find(afterNext) == prior.end()) continue;
            posterior3[curr + next + afterNext] += 1;
        }
    }
    in.close();
}

void save2file(const string &fileName, const unordered_map<string, int> &data){
    ofstream out(fileName);
    if(!out.is_open()){
        cerr << "Failed to open file " << fileName << endl;
        exit(0); 
    }
    for(auto entry : data)
        out << entry.first << ' ' << entry.second << endl;
    out.close();
}

void load_from_file(const string &fileName, unordered_map<string, int> &data){
    ifstream in(fileName);
    if(!in.is_open()){
        cerr << "Failed to open file " << fileName << endl;
        exit(0);
    }
    string key;
    int value;
    while(in >> key >> value)
        data[key] = value;
}

int _main(){
    unordered_map<string, int> prior;
    unordered_map<string, int> posterior;
    unordered_map<string, int> posterior3;
    unordered_map<string, vector<string>> pinyin2han;
    load_valid_char(prior);
    bulid_pinyin2han_map(pinyin2han);
    for(int ix = 0; ix != NUM_FILE; ++ix){
        compute_prior_posterior(path + corpus_files[ix] + ".txt", prior, posterior, posterior3);
        cout << "Finish processing file " << corpus_files[ix] + ".txt" << endl;
    }
    string priorFile = "model/prior.txt";
    string posteriorFile = "model/posterior.txt";
    string posterior3File = "model/posterior3.txt";
    save2file(priorFile, prior);
    save2file(posteriorFile, posterior);
    save2file(posterior3File, posterior3);
}
