#include "predict.h"
#include "model.h"

// exec from command line: bin/pinyin [-n2] data/input_file data/output_file
int main(int argc, char* argv[]){
    // input processing
    string inputFile, outputFile, modeStr;
    Predictor *pred;
    int mode = 3;
    if(argc == 4){
        modeStr = argv[1];
        inputFile = argv[2];
        outputFile = argv[3];
        if(modeStr == "-n2")  mode = 2;
        else if(modeStr == "-n3");
        else{
            cerr << "Unrecognized mode identifier." << endl;
            cerr << "Use mode 3 by default" << endl;
        }
    }
     else if(argc == 3){
        inputFile = argv[1];
        outputFile = argv[2];
    }
    else{
        cerr << "Invalid arguments." << endl;
        cerr << "Program aborts." << endl;
        exit(0);
    }
    pred = new Predictor(mode);

    // predict
    string input, output;
    ifstream in(inputFile);
    if(!in.is_open()){
        cerr << "Failed to open file " << inputFile << endl;
        cerr << "Program aborts." << endl;
        exit(0);
    }
    ofstream out(outputFile);
    if(!out.is_open()){
        cerr << "Failed to open file " << outputFile << endl;
        cerr << "Program aborts." << endl;
        exit(0);
    }
    while(getline(in, input)){
        if(mode == 3) output = pred->predict3(input);
        else          output = pred->predict2(input);
        out << output << endl;
    }

    in.close();
    out.close();
    return 0;
}
