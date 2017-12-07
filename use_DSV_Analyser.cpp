//
// Created by Sn0w1eo on 07.12.2017.
//
#include "DSV_Analyser.h"
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    clock_t start = clock();
    DSV_Analyser obj(R"(YOUR PATH TO DSV FILE)", ',' );
    obj.Analyse();
    clock_t finish = clock();
    double elapsed_time = double(finish-start)/1000;
    cout << endl << "TIME ELAPSED IN SEC: " << elapsed_time << endl;
}
