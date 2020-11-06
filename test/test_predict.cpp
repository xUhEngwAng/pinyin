#include "../src/predict.h"
#include <fstream>
#include <iostream>

// exec from command line: ./test_predict inputFile
int main(int argc, char* argv[]){
    // assert(argc == 2);
    string inputFile = argv[1];
    string input, output;
    ifstream in(inputFile);
    if(!in.is_open()){
        cerr << "Failed to open file " << inputFile << endl;
        exit(0);
    }
    getline(in, input);
    cout << input << endl;
    
    Predictor predictor;
    output = predictor.predict(input);
    cout << output << endl;
    return 0;
}
