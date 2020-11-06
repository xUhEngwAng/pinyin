#include "../src/model.h"

unordered_map<string, int> prior;
unordered_map<string, int> posterior;
unordered_map<string, int> posterior3;
unordered_map<string, vector<string>> pinyin2han;

int main(){
    load_valid_char(prior);
    bulid_pinyin2han_map(pinyin2han);
    string input_file = "/mnt/d/user/Documents/projects/pinyin/test/test_model.txt";
    compute_prior_posterior(input_file, prior, posterior, posterior3);
    //prior test
    cout << "Test Prior." << endl;
    for(auto entry : prior){
        if(entry.second)
            cout << entry.first << ": " << entry.second << endl;;
    }

    //posterior test
    cout << "Test Posterior." << endl;
    for(auto entry : posterior){
        cout << entry.first << ": " << entry.second << endl;;
    }

    // save2file test
    string priorFile = "/mnt/d/user/Documents/projects/pinyin/model/prior.txt";
    string posteriorFile = "/mnt/d/user/Documents/projects/pinyin/model/posterior.txt";
    save2file(priorFile, prior);
    save2file(posteriorFile, posterior);

    // predict::load_from_file test
    unordered_map<string, int> priorMap;
    unordered_map<string, int> posteriorMap;
    load_from_file(priorFile, priorMap);
    load_from_file(posteriorFile, posteriorMap);
    cout << "Test Prior." << endl;
    for(auto entry : prior){
        if(entry.second)
            cout << entry.first << ": " << entry.second << endl;;
    }
    cout << "Test Posterior." << endl;
    for(auto entry : posterior){
        cout << entry.first << ": " << entry.second << endl;;
    }
}
