#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// compile this file from command line instead of Makefile
// g++ -o bin/evaluate src/evaluate.cpp

int main(int argc, char* argv[]){
    string predFile, ansFile;
    if(argc < 3){
        cerr << "To few arguments, THREE required." << endl;
        exit(0);
    }
    if(argc > 3){
        cerr << "To many arguments, THREE required." << endl;
        exit(0);
    }

    predFile = argv[1];
    ansFile  = argv[2];
    ifstream inPred(predFile), inAns(ansFile);
    if(!inPred.is_open()){
        cerr << "Failed to open file " << predFile << endl;
        exit(0);
    }
    if(!inAns.is_open()){
        cerr << "Failed to open file " << ansFile << endl;
        exit(0);
    }

    // evaluate prediction results

    string pred, ans;
    int charTotal = 0, charCnt = 0;
    int lineTotal = 0, lineCnt = 0;
    while(inPred >> pred && inAns >> ans){
        if(pred == ans) lineCnt += 1;
        lineTotal += 1;
        for(int ix = 0; ix != pred.size(); ix += 3){
            if(pred.substr(ix, 3) == ans.substr(ix, 3)) charCnt += 1;
            charTotal += 1;
        }
    }
    cout << "Prediction accuracy per char: " << charCnt * 100/(float)charTotal << "%" << endl;
    cout << "Prediction accuracy per line: " << lineCnt * 100/(float)lineTotal << "%" << endl;

    inPred.close();
    inAns.close();
}
