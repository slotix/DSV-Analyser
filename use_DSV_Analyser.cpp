//
// Created by Sn0w1eo on 07.12.2017.
//
#include "DSV_Analyser.h"
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    string filename = R"(YOUR FULL PATH TO FILE)";

    cout << "ANALYSE STARTED..." << endl;
    clock_t start = clock();
    DSV_Analyser obj(filename.c_str() , ',' );
    obj.Analyse();
    clock_t finish = clock();
    double elapsed_time = double(finish-start)/1000;
    cout << "TIME ELAPSED IN SEC: " << elapsed_time << endl;
}
