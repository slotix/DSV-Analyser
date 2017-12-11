//
// Created by Sn0w1eo on 07.12.2017.
//
#include "DSV_Analyser.h"
#include <iomanip>
#include <iostream>

using namespace std;

int main() {
//    string filename = R"(D:\Trash\csv_files\10Million.csv)";
    string filename = R"(D:\Trash\csv_files\sample.csv)";

    cout << "ANALYSE STARTED..." << endl;
    clock_t start = clock();
    DSV_Analyser obj(filename.c_str() , ',', '.' );
    obj.Analyse(true);
    clock_t finish = clock();
    double elapsed_time = double(finish-start)/1000;
    cout << "TIME ELAPSED IN SEC: " << elapsed_time << endl;
    cout << "STATISTICS:" << endl;
    cout << "Total Columns:" << obj.Columns.size() << endl << endl;
    cout << setw(10) << right << "#" << "  " << setw(60) << left << "TITLE"
         << setw(10) << right << "LENGTH" << setw(10) << "TYPE" << endl;
    cout << setw(92) << setfill('-') << '-' << setfill(' ') << endl;
    for (int i=0; i < obj.Columns.size(); i++) {
        cout << setw(10) <<  right << i << "  " << setw(60) << left << setfill('.') << obj.Columns[i].Title
             << setw(10) << right << obj.Columns[i].Length << setw(10) << setfill(' ');
        if (obj.Columns[i].Type & DSV_TYPES::TEXT_TYPE) {
            cout << "Text";
        } else if(obj.Columns[i].Type & DSV_TYPES::DOUBLE_TYPE) {
            cout << "Double";
        } else if(obj.Columns[i].Type & DSV_TYPES::INTEGER_TYPE) {
            cout << "Integer";
        } else {
            cout << "Undefined";
        }
        cout << endl;
    }
    cout << endl;
}
